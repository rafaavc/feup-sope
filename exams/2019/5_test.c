#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define MAX_STRING 512

#define READ 0
#define WRITE 1

int main(int argc, char * argv [])  {
    if (argc != 2) {
        return 1;
    }

    char * outFilename = argv[1];
    int outFd = open(outFilename, O_CREAT|O_WRONLY|O_APPEND, 0660);

    int in[2], out[2];
    if (pipe(in) == -1) return 1;
    if (pipe(out) == -1) return 1;

    int pid = fork();

    if (pid == -1) return 1;
    else if (pid == 0) { // child
        close(in[WRITE]);
        close(out[READ]);
        dup2(in[READ], STDIN_FILENO);
        dup2(out[WRITE], STDOUT_FILENO);
        execlp("bc", "bc", "-qi", NULL);
        return 1;
    }

    // parent
    close(in[READ]);
    close(out[WRITE]);

    size_t n;

    /*char * line = malloc(MAX_STRING);
    char * writeToFile = malloc(MAX_STRING);
    while((n = read(STDIN_FILENO, line, MAX_STRING)) != 0) {
        if (n == -1) return 1;
        line[n-1] = '\0'; // substitutes '\n' for '\0'
        if (strlen(line) == 0) break;

        write(in[WRITE], line, strlen(line));
        sprintf(writeToFile, "%s = ", line);

        n = read(out[READ], line, MAX_STRING);

        line[n-1] = '\0';
        strcat(writeToFile, line);
        strcat(writeToFile, "\n");

        if (write(outFd, writeToFile, strlen(writeToFile)) == -1) return 1;
    }*/

    char* line = NULL;
    close(in[READ]);
    close(out[WRITE]);
    FILE* f1 = fdopen(in[WRITE], "w");
    FILE* f2 = fdopen(out[READ], "r");

    while(getline(&line, &n, stdin) != -1){
        fprintf(f1, "%s", line);
        fflush(f1);
        getline(&line, &n, f2);
        line[strlen(line)-1] = '\0';
        write(outFd, line, strlen(line));
        write(outFd, " = ", 3);
        getline(&line, &n, f2);
        write(outFd, line, strlen(line));
    }

    free(line);
    close(outFd);
    return 0;
}