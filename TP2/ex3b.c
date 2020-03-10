#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 512

int main(int argc, char * argv[]) {
    int f1;

    if (argc < 2) {
        write(STDOUT_FILENO, "You didn't specify the right arguments!\n", 43);
        exit(1);
    }

    f1 = open(argv[1], O_RDONLY);
    if (f1 == -1) {
        write(STDOUT_FILENO, "Error opening file ", 19);
        write(STDOUT_FILENO, argv[1], strlen(argv[1]));
        write(STDOUT_FILENO, "\n", 1);
        exit(2);
    }

    char buffer[BUFFER_SIZE]; int nr, nw;

    while ((nr = read(f1, buffer, BUFFER_SIZE)) > 0) {
        if ((nw = write(STDOUT_FILENO, buffer, nr)) <= 0 || nw != nr) {
            perror(argv[2]);
            close(f1);
            exit(4);
        }
    }

    close(f1);

    exit(0);
}