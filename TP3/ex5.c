#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pid, pid1, ret;
    pid = fork();
    switch(pid) {
        case -1:
            write(STDOUT_FILENO, "Error forking!\n", 15);
            exit(1);
            break;
        case 0:
            // child
            pid1 = fork();
            switch(pid1) {
                case -1:
                    write(STDOUT_FILENO, "Error forking!\n", 15);
                    exit(1);
                    break;
                case 0:
                    // child
                    write(STDOUT_FILENO, "Hello ", 6);
                    break;
                default:
                    // parent
                    wait(&ret);
                    if (ret != 0) {
                        write(STDOUT_FILENO, "Child disappointed parent 2!\n", 15);
                    }
                    write(STDOUT_FILENO, "my ", 3);
                    break;

            }
            break;
        default:
            // parent
            wait(&ret);
            if (ret != 0) {
                write(STDOUT_FILENO, "Child disappointed parent 1!\n", 15);
            }
            write(STDOUT_FILENO, "friends!\n", 9);
            break;
    }
    exit(0);
}