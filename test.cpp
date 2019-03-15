#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    int fd = open(".", O_RDONLY);
    printf("%d\n", fd);
    char tmp[11234];
    int a  = read(fd, tmp, 10);
    printf("%d\n", a);
    puts(tmp);
}