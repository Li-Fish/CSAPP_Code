#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NRECS 16
#define NULLUT ((struct utmp *) NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

int utmp_open(char *file_name) {
    fd_utmp = open(file_name, O_RDONLY);
    cur_rec = num_recs = 0;
    return fd_utmp;
}

int utmp_reload() {
    int amt_read;
    amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
    num_recs = amt_read / UTSIZE;
    cur_rec = 0;
    return num_recs;
}

struct utmp *utmp_next() {
    struct utmp *recp;
    if (fd_utmp == -1) {
        return NULLUT;
    }
    if (cur_rec == num_recs && utmp_reload() == 0) {
        return NULLUT;
    }
    recp = (struct utmp*) &utmpbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
}

void utmp_close() {
    if (fd_utmp != -1) {
        close(fd_utmp);
    }
}

#define SHOWHOST

void showtime(long);
void show_info(struct utmp *);

int main() {
    struct utmp *utbufp;
    if (utmp_open(UTMP_FILE) == -1) {
        perror(UTMP_FILE);
        exit(1);
    }
    while ((utbufp = utmp_next()) != NULLUT) {
        show_info(utbufp);
    }
    utmp_close();
}

void show_info(struct utmp *utbufp) {
    if (utbufp->ut_type != USER_PROCESS) return;
    printf("%-8.8s ", utbufp->ut_name);
    printf("%-8.8s ", utbufp->ut_line);
    showtime(utbufp->ut_time);
#ifdef SHOWHOST
    printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}

void showtime(long timeval) {
    char *cp;
    cp = ctime(&timeval);
    printf("%12.12s", cp + 4);
}