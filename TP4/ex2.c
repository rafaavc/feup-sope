
#include "signal.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"

void sigint_handler(int signo) {
    printf("Entering SIGINT handler ...\n");
    sleep(10);
    printf("Exiting SIGINT handler ...\n");
}

void sigterm_handler(int signo) {
    printf("Entering SIGTERM handler ...\n");
    sleep(1);
    printf("Exiting SIGTERM handler ...\n");
}

int main(void) {
    struct sigaction sigintAction;
    struct sigaction sigtermAction;
    
    sigintAction.sa_handler = sigint_handler;
    sigemptyset(&sigintAction.sa_mask);
    sigintAction.sa_flags = 0;

    sigtermAction.sa_handler = sigterm_handler;
    sigemptyset(&sigtermAction.sa_mask);
    sigtermAction.sa_flags = 0;

    if (sigaction(SIGINT,&sigintAction,NULL) < 0) {
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }

    if (sigaction(SIGTERM,&sigtermAction,NULL) < 0) {
        fprintf(stderr,"Unable to install SIGTERM handler\n");
        exit(1);
    }

    printf("Try me with CTRL-C ...\n");
    while(1) pause();
    exit(0);
}

/*
a) Quando o handler está a ser executado, qualquer 
que seja o número de sinais SIGINT transmitidos, apenas um deles fica
na "queue" para ser processado após o término da execução do handler.

b) Termina o processo. SIGTERM tem mais prioridade que SIGINT

c) É interrompida a execução do sigint_handler, sendo retomada após o final
da execução do sigterm_handler.
*/

