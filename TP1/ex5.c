#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp) {
    /*unsigned i = 0;
    char holder[5];
    while (1) {
        if (envp[i] == NULL) {
            break;
        }
        for (unsigned l = 0; l < 5; l++) {
            holder[l] = envp[i][l];
        }
        if (strcmp(holder, "USER=") == 0) {
            char name[strlen(envp[i])-5];
            for (unsigned j = 5; j < strlen(envp[i]); j++) {
                name[j-5] = envp[i][j];
            }
            printf("Hello %s!\n", name);
            break;
        }
        i++;
    }*/
    //OR:
    printf("Hello %s!\n", getenv("USER"));
    // Shell: export & unset
    return 0;
}