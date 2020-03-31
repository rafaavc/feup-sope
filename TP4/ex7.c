#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>


void sendTerminationToChildren(pid_t pids[], int count) {
    for (int i = 0; i < count; i++) {
        if (pids[i] != 0) {
            kill(pids[i], SIGKILL);
        }
    }
}

pid_t pidWaited = 0;
int status;

void sig_handler(int signo) {
    if (signo == SIGCHLD) {
        pidWaited = waitpid(-1, &status, WNOHANG);
    }
}


int main(int argc, const char ** argv) {
    if (argc < 2) {
        printf("Usage:\n\nex7 prog*\nexecutes all progs specified\n");
        exit(0);
    }

    struct sigaction action;

    action.sa_handler = &sig_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGCHLD, &action, NULL);

    pid_t pid, pids[argc-1];
    for (int i = 0; i < argc-1; i++) {
        pids[i] = 0;
    }

    for (int i = 1; i < argc; i++) {
        pid = fork();

        if (pid == -1) {
            printf("Error forking\n");
            sendTerminationToChildren(pids, argc-1);
            abort();
        } else if (pid == 0) {
            int ret = execlp((const char*) argv[i], (const char *) argv[i], (char *) NULL);
            if (ret == -1) {
                printf("You need to give a correct program name that is in the path env variable!\n");
                sendTerminationToChildren(pids, argc-1);
                abort();
            }
        } else {
            printf("Created process with pid=%d for \"%s\"\n", pid, argv[i]);
            pids[i-1] = pid;
        }
    }

    int count = 0;
    while (count != argc-1) {
        if (pidWaited != 0) {
            for (int i = 0; i < argc-1; i++) {
                if (pids[i] == pidWaited) {
                    pids[i] = 0;
                }
            }
            if (status != 0) {
                sendTerminationToChildren(pids, argc-1);
                printf("Process with pid=%d terminated with status=%d. All other processes were terminated.\n", pidWaited, status);
            }
            pidWaited = 0;
            count++;
        }
    }

    printf("Execution finished\n");

    exit(0);
}

