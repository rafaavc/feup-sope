#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 512
#define ARRAY_SIZE 25

int main() {
    char buffer[BUFFER_SIZE];
    char * tok;
    char * arrayHolder[ARRAY_SIZE];

    int nr = read(STDIN_FILENO, &buffer, BUFFER_SIZE);
    if (nr <= 0) {
        printf("Error reading\n");
        exit(1);
    }
    tok = strtok(buffer, " ");

    unsigned i = 0;
    while (tok != NULL) {
        arrayHolder[i] = tok;
        tok = strtok(NULL, " ");
        i++;
        if (i == 25) {
            printf("Array size exceeded\n");
            break;
        }
    }

    char delim[] = " ";

    for (unsigned j = 0; j < i; j++) {
        write(STDOUT_FILENO, arrayHolder[j], strlen(arrayHolder[j]));
        write(STDOUT_FILENO, delim, strlen(delim));
    }


    exit(0);
}