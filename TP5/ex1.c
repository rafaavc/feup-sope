#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define MAX_BUFFER_SIZE 25

struct myNumbers {
    int n1;
    int n2;
};

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("Error pipeing\n");
        exit(1);
    }

    pid_t pid;
    char buffer[MAX_BUFFER_SIZE];
    int n;
    struct myNumbers numbers;
    switch(pid = fork()) {
        case 0:
            // child
            close(pipefd[WRITE]);
            read(pipefd[READ], &numbers, sizeof(numbers));
            int n1 = numbers.n1;
            int n2 = numbers.n2;
            printf("%d*%d=%d\n", n1, n2, n1*n2);
            printf("%d+%d=%d\n", n1, n2, n1+n2);
            printf("%d-%d=%d\n", n1, n2, n1-n2);
            if (n2 == 0) {
                printf("You can't divide a number by 0!\n");
            } else {
                printf("%d/%d=%f\n", n1, n2, (float) n1/n2);
            }
            break;
        case -1:
            printf("Error forking\n");
            exit(1);
            break;
        default:
            // parent

            close(pipefd[READ]);
            n = read(STDIN_FILENO, &buffer, MAX_BUFFER_SIZE);
            //write(pipefd[WRITE], buffer, n);
            numbers.n1 = atoi(buffer);
            n = read(STDIN_FILENO, &buffer, MAX_BUFFER_SIZE);
            //write(pipefd[WRITE], buffer, n);
            numbers.n2 = atoi(buffer);
            write(pipefd[WRITE], &numbers, sizeof(numbers));
            break;
    }


    wait(NULL);

    exit(0);
}

