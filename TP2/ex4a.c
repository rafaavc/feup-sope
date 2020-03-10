#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
    if (argc < 1) {
        write(STDOUT_FILENO, "Incorrect arguments given!\nUsage: write <filedir>\n", 52);
        exit(1);
    }

    int nr, nw;
    char buffer[BUFFER_SIZE];

    int f;
    if ((f = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0644)) == -1) {
        write(STDOUT_FILENO, "Error opening file", 18);
        exit(2);
    }

    while((nr = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        if ((nw = write(f, buffer, nr)) <= 0) {
            write(STDOUT_FILENO, "", 52);
            exit(3);
        }
    }
    exit(0);
}