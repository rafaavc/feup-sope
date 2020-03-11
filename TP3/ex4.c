#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pid = fork();
    switch(pid) {
        case -1:
            write(STDOUT_FILENO, "Error forking!\n", 15);
            exit(1);
            break;
        case 0:
            // child
            //write(STDOUT_FILENO, "Hello ", 6);
            write(STDOUT_FILENO, "world!\n", 7);
            break;
        default:
            //parent
            /*wait(NULL);
            write(STDOUT_FILENO, "world!\n", 7);*/
            write(STDOUT_FILENO, "Hello ", 6);
            break;
    }
    exit(0);
}