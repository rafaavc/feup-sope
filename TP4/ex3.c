#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

unsigned cresc = 1;

void sigusr_handler(int signo) {
    if (signo == SIGUSR1) {
        cresc = 1;
    } else if (signo == SIGUSR2) {
        cresc = 0;
    }
}

void sigchild_handler(int signo) {
    raise(SIGINT);
}

int main() {
    int v = 0;
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        fprintf(stderr,"Unable to FORK\n");
        exit(1);
    }

    if (pid > 0) {
        printf("Parent\n");

        struct sigaction sigchildAction;

        sigchildAction.sa_handler = sigchild_handler;
        sigemptyset(&sigchildAction.sa_mask);
        sigchildAction.sa_flags = 0;

        if (sigaction(SIGCHLD,&sigchildAction,NULL) < 0) {
            fprintf(stderr,"Unable to install SIGCHILD handler\n");
            exit(1);
        }

        time_t t;
        srand((unsigned) time(&t));

        int randNo;

        while(1) {
            randNo = rand();
            if (randNo % 2 == 1) {
                kill(pid, SIGUSR1); // numbers increase
            } else if (randNo % 7 == 0) {
                kill(pid, SIGUSR2); // numbers decrease
            }
            sleep(2);
        }

    } else if (pid == 0) {
        // child

        struct sigaction action;

        action.sa_handler = sigusr_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;

        if (sigaction(SIGUSR1,&action,NULL) < 0) {
            fprintf(stderr,"Unable to install SIGUSR1 handler\n");
            exit(1);
        }

        if (sigaction(SIGUSR2,&action,NULL) < 0) {
            fprintf(stderr,"Unable to install SIGUSR2 handler\n");
            exit(1);
        }

        int temp_sleep;
        unsigned count = 0;
        while(count < 50) {
            temp_sleep = 1;
            printf("%d\n", v);
            if (cresc) v++;
            else v--;
            while((temp_sleep=sleep(temp_sleep)) != 0);
            count++;
        }
    }

    exit(0);
}





