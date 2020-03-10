#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 10

int main(int argc, char *argv[]) {
    if (argc < 3) {
        write(STDOUT_FILENO, "Wrong arguments given.\n", 24);
        exit(-1);
    }

    char c[BUFFER_SIZE];
    int nr, nw;

    FILE * f1 = fopen(argv[1], "r");
    if (f1 == NULL) exit(3);
    FILE * f2 = fopen(argv[2], "w");
    if (f2 == NULL) {
        fclose(f1);
        exit(4);
    }

    

    while ((nr = fread(c, sizeof(char), BUFFER_SIZE, f1)) > 0) {
        if ((nw = fwrite(c, sizeof(char), nr, f2)) <= 0) {
            write(STDOUT_FILENO, "Error writing to file ", 22);
            write(STDOUT_FILENO, argv[2], strlen(argv[2]));
            write(STDOUT_FILENO, "\n", 1);
            fclose(f1);
            fclose(f2);
            exit(2);
        }
    }

    fclose(f1);
    fclose(f2);


    exit(0);
}