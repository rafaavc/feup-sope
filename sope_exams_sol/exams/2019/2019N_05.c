#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define ERROR(s) { perror(s); return 1; }

#define LINE_SIZE 1024

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: ./2019N_05 FILE");
        return 1;
    }
    char *output_filepath = argv[1];
    int fd_out = open(output_filepath, O_CREAT|O_WRONLY|O_APPEND);
    if(fd_out == -1)                      ERROR("open output_filepath");
    if(dup2(fd_out, STDOUT_FILENO) == -1) ERROR("dup2 fd_out");

    int to[2]; if(pipe(to)) ERROR("Could not create pipe to");
    int fr[2]; if(pipe(fr)) ERROR("Could not create pipe fr");

    pid_t pid = fork();
    if(pid == 0) /** Child */ {
        if(close(to[1]))                  ERROR("close to[1]"); // close write end
        if(dup2(to[0],STDIN_FILENO) ==-1) ERROR("dup2 to[0]" ); // stdin point to to[0]
        if(close(fr[0]))                  ERROR("close fr[0]"); // close read end
        if(dup2(fr[1],STDOUT_FILENO)==-1) ERROR("dup2 fr[1]" ); // stdout point to fr[1]
        execlp("bc", "bc", "-qi", NULL);
        perror("Call to exec failed");
        return 1;
    }

    if(close(to[0]))                                       ERROR("close to[0]" );
    FILE *fd_to_bc = fdopen(to[1],"w"); if(fd_to_bc==NULL) ERROR("fdopen to[1]");
    if(close(fr[1]))                                       ERROR("close fr[1]" );
    FILE *fd_fr_bc = fdopen(fr[0],"r"); if(fd_fr_bc==NULL) ERROR("fdopen fr[0]");

    char *line = NULL;
    size_t n = 0;
    while(getline(&line, &n, stdin) != -1) {
        fprintf(fd_to_bc, "%s", line);
        fflush(fd_to_bc);

        getline(&line, &n, fd_fr_bc); 
        line[strlen(line)-1] = '\0';
        printf("%s = ", line);
        getline(&line, &n, fd_fr_bc);
        printf("%s", line);
    }
    free(line);

    return 0;
}