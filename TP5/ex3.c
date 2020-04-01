#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

#define MAX_BUFFER_SIZE 10240


void version1(char * filename) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("Error pipeing\n");
        exit(2);
    }
    char buffer[MAX_BUFFER_SIZE];
    pid_t pid;
    switch(pid = fork()) {
        case 0:
            // child
            close(pipefd[WRITE]);
            dup2(pipefd[READ], STDIN_FILENO);
            
            execlp("sort", "sort", NULL);

            break;
        case -1:
            printf("Error forking\n");
            exit(1);
            break;
        default:
            // parent
            close(pipefd[READ]);
            int fd = open(filename, O_RDONLY);
            if (fd == -1) {
                printf("You need to insert a valid filename!\n");
                exit(3);
            }
            int n = read(fd, &buffer, MAX_BUFFER_SIZE);
            write(pipefd[WRITE], &buffer, n);
            break;
    }
}

void version2(const char * filename) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("Error pipeing\n");
        exit(2);
    }
    pid_t pid;
    switch(pid = fork()) {
        case 0:
            // child
            close(pipefd[WRITE]);
            dup2(pipefd[READ], STDIN_FILENO);
            
            execlp("sort", "sort", NULL);

            break;
        case -1:
            printf("Error forking\n");
            exit(1);
            break;
        default:
            // parent
            close(pipefd[READ]);
            int fd = open(filename, O_RDONLY);
            if (fd == -1) {
                printf("You need to insert a valid filename!\n");
                exit(3);
            }
            dup2(pipefd[WRITE], STDOUT_FILENO);
            execlp("cat", "cat", filename, NULL);
            break;
    }
}

int main(int argc, char ** argv) {
    if (argc < 3) {
        printf("Usage:\n\nex3 versionNo fileName\n\nUses the version versionNo of the program to order the lines the file with name filename\nversion 1: standard\nversion 2: using cat\n");
        exit(1);
    }

    switch(atoi(argv[1])) {
        case 1:
            version1(argv[2]);
            break;
        case 2:
            version2(argv[2]);
            break;
        default:
            printf("Wrong versionNo\n");
            exit(1);
            break;
    }

    exit(0);
}


