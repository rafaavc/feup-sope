#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char s[256];
    fgets(s, 256, stdin);

    char * tokens[15];

    unsigned i = 0;
    while (1) {
        tokens[i] = malloc(sizeof(char)*50);
        tokens[i] = strtok(s, " ");

        i++;
    }

    i = 0;
    while(1) {
        if (tokens[i] == NULL) {
            break;
        }

        printf("%s\n", tokens[i]);
        i++;
    }

    exit(0);
}

