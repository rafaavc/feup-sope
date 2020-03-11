#include <stdio.h>
#include <unistd.h>


int main(void) {
    //write(STDOUT_FILENO,"1",1);
    printf("1\n"); // Without the newline the buffer keeps the number 1 and it is printed in the child process.
    // With the newline, the buffer is cleared and the number 1 doesn't appear in the child process.
    if(fork() > 0) {
        /*write(STDOUT_FILENO,"2",1);
        write(STDOUT_FILENO,"3",1);*/
        printf("2");
        printf("3");
    } else {
        /*write(STDOUT_FILENO,"4",1);
        write(STDOUT_FILENO,"5",1);*/
        printf("4");
        printf("5");
    }
    //write(STDOUT_FILENO,"\n",1);
    printf("\n");
    return 0;
}

