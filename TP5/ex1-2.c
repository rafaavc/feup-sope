#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define READ 0
#define WRITE 1
#define MAX_BUFFER_SIZE 25

struct myNumbers {
    int n1;
    int n2;
};

struct myResults {
    int sumRes;
    int multRes;
    int subRes;
    float divRes;
    bool divBy0;
};

int main() {
    int pipefd[2], pipefd2[2];
    if (pipe(pipefd) == -1) {
        printf("Error pipeing1\n");
        exit(1);
    }
    if (pipe(pipefd2) == -1) {
        printf("Error pipeing2\n");
        exit(1);    
    }

    pid_t pid;
    char buffer[MAX_BUFFER_SIZE];
    struct myNumbers numbers;
    struct myResults results;
    switch(pid = fork()) {
        case 0:
            // child
            close(pipefd[WRITE]);
            close(pipefd2[READ]);

            read(pipefd[READ], &numbers, sizeof(numbers));

            int n1 = numbers.n1;
            int n2 = numbers.n2;
            results.multRes = n1*n2;
            results.sumRes = n1+n2;
            results.subRes = n1-n2;
            if (n2 == 0) {
                results.divBy0 = true;
            } else {
                results.divBy0 = false;
                results.divRes = (float) n1/n2;
            }

            write(pipefd2[WRITE], &results, sizeof(results));
            break;
        case -1:
            printf("Error forking\n");
            exit(1);
            break;
        default:
            // parent
            close(pipefd[READ]);
            close(pipefd2[WRITE]);

            read(STDIN_FILENO, &buffer, MAX_BUFFER_SIZE);
            //write(pipefd[WRITE], buffer, n);
            numbers.n1 = atoi(buffer);
            read(STDIN_FILENO, &buffer, MAX_BUFFER_SIZE);
            //write(pipefd[WRITE], buffer, n);
            numbers.n2 = atoi(buffer);
            write(pipefd[WRITE], &numbers, sizeof(numbers));

            
            read(pipefd2[READ], &results, sizeof(results));
            printf("Results:\nsum = %d\nsub = %d\nmul = %d\n", results.sumRes, results.subRes, results.multRes);
            if (results.divBy0) {
                printf("division by 0 is impossible for me :(\n");
            } else {
                printf("div=%f\n", results.divRes);
            }
            break;
    }


    wait(NULL);

    exit(0);
}

