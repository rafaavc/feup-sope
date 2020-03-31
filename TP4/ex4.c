#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

pid_t pid;
int status;

void sigchild_handler(int signo) {
    pid = waitpid(-1, &status, WNOHANG);
    printf("sigchildhandler\n");
}

int main(void) {
    pid_t pid1;
    int i, n;

    struct sigaction sigchildAction;

    sigchildAction.sa_handler = sigchild_handler;
    sigemptyset(&sigchildAction.sa_mask);
    sigchildAction.sa_flags = 0;

    if (sigaction(SIGCHLD, &sigchildAction, NULL)!=0) {
        printf("Error while setting sigchild handler.\n");
    }   


    for (i=1; i<=3; i++) {
        pid1=fork();
        if (pid1 == 0){
            printf("CHILD no. %d (PID=%d) working ... \n",i,getpid());
            sleep(i*7); // child working ...
            printf("CHILD no. %d (PID=%d) exiting ... \n",i,getpid());
            raise(SIGCHLD);
            exit(0);
        }
    }

    for (i=1 ;i<=4; i++ ) {
        printf("PARENT: working hard (task no. %d) ...\n",i);
        n=20; while((n=sleep(n))!=0);
        printf("PARENT: end of task no. %d\n",i);
        printf("PARENT: waiting for child no. %d ...\n",i);

        if (pid != -1)
            printf("PARENT: child with PID=%d terminated with exit code %d\n",pid,WEXITSTATUS(status));
    }
    exit(0);
}