#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main(int argc, char ** argv) {
    if (argc < 3) {
        printf("Usage:\nex3 lsArg grepArg\n");
        exit(1);
    }

    int pipefd1[2], pipefd2[1];
    
    if (pipe(pipefd1) == -1) {
        printf("Error pipeing\n");
        exit(2);
    }

    pid_t pid;
    switch(pid=fork()) {
        case 0:
            close(pipefd1[WRITE]);
            dup2(pipefd1[READ], STDIN_FILENO);
            close(pipefd1[READ]);

            if (pipe(pipefd2) == -1) {
                printf("Error pipeing\n");
                exit(2);
            }
            switch(pid=fork()) {
                case 0:
                    // child2
                    close(pipefd2[WRITE]);

                    dup2(pipefd2[READ], STDIN_FILENO);

                    execlp("sort", "sort", NULL);

                    break;
                case -1:
                    printf("Error forking\n");
                    exit(1);
                    break;
                default:
                    // parent2
                    close(pipefd2[READ]);
                    dup2(pipefd2[WRITE], STDOUT_FILENO);

                    execlp("grep", "grep", argv[2], NULL);

                    break;
            }
            break;
        case -1:
            printf("Error forking\n");
            exit(1);
            break;
        default:
            // parent1
            close(pipefd1[READ]);

            dup2(pipefd1[WRITE], STDOUT_FILENO);

            execlp("ls", "ls", argv[1], "-laR", NULL);

            break;
    }

    exit(0);
}
