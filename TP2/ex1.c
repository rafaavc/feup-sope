#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_PASSLEN 10

int main() {
    struct termios oldterm, term;
    tcgetattr(STDIN_FILENO, &oldterm);

    char pass[MAX_PASSLEN], ch;

    term = oldterm;
    term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
    write(STDIN_FILENO, "\nPassword? ", 11);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
    
    int i = 0;
    while (read(STDIN_FILENO, &ch, 1) && i < MAX_PASSLEN && ch != '\n') {
        pass[i] = ch;
        i++;
        write(STDOUT_FILENO,  "*", 1);
    }
    pass[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm);

    write(STDOUT_FILENO, "\nPassword: ", 12);
    write(STDOUT_FILENO, pass, strlen(pass));
    write(STDOUT_FILENO, "\n", 1);

    exit(0);
}