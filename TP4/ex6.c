#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>


pid_t pid = 0;
bool canExit = false;

void sig_handler(int signo) {
    if (signo == SIGALRM && pid != 0) {
        kill(pid, SIGKILL);
    } else if (signo == SIGCHLD) {
        waitpid(-1, NULL, WNOHANG);
    } else {
        printf("something went wrong\n");
        exit(1);
    }
    canExit = true;
}

void print_usage() {
    printf("Usage:\n\ex6 t prog prog_arg1 prog_arg2 ...\nrun prog with prog_arg* for a maximum of t seconds\n");
}

int main(int argc, char ** argv) {
    if (argc < 3) {
        print_usage();
        exit(0);
    }
    struct sigaction action;

    action.sa_handler = &sig_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);
    sigaction(SIGCHLD, &action, NULL);

    unsigned int t = (unsigned int) atoi(argv[1]);

    pid = fork();
    if (pid == -1) {
        printf("Error forking\n");
        abort();
    } else if (pid == 0) {
        char * progargv[argc-1];
        for (int i = 2; i < argc; i++) {
            progargv[i-2] = argv[i];
        }
        int ret = execvp((const char*)argv[2], (char *const*) progargv);
        if (ret == -1) {
            printf("You need to give a correct program name and arguments!\n");
            abort();
        }
    } else {
        alarm(t);
    }

    while(!canExit);


    exit(0);
}
