#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

pid_t pid = 0;
int status;

void sigchild_handler(int signo) {
    pid = waitpid(-1, &status, WNOHANG);
}

int main(void) {
    pid_t pid1;
    int i;

    struct sigaction sigchildAction;

    sigchildAction.sa_handler = sigchild_handler;
    sigemptyset(&sigchildAction.sa_mask);
    sigchildAction.sa_flags = 0;

    if (sigaction(SIGCHLD, &sigchildAction, NULL)!=0) {
        printf("Error while setting sigchild handler.\n");
        exit(1);
    }   


    for (i=1; i<=10; i++) {
        pid1=fork();
        if (pid1 == 0){
            printf("CHILD no. %d (PID=%d) working ... \n",i,getpid());
            sleep(7+i); // child working ...
            printf("CHILD no. %d (PID=%d) exiting ... \n",i,getpid());
            raise(SIGCHLD);
            exit(0);
        }
    }
    int count = 0;
    while(count != 10) {
        if (pid != 0) {
            if (pid > 0) {
                printf("Child with pid=%d just terminated with status code=%d.\n", pid, status);
            } else if (pid == -1) {
                printf("ERRNO: %d\n\n", errno);
            }
            pid = 0;
        }
    }

    /*for (i=1 ;i<=4; i++ ) {
        printf("PARENT: working hard (task no. %d) ...\n",i);
        n=20; while((n=sleep(n))!=0);
        printf("PARENT: end of task no. %d\n",i);
        printf("PARENT: waiting for child no. %d ...\n",i);

        if (pid != -1)
            printf("PARENT: child with PID=%d terminated with exit code %d\n",pid,WEXITSTATUS(status));
    }*/
    exit(0);
}