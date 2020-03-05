#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("You didn't specify the required argument.\n");
        return 1;
    } 
    
    if (argc > 2) {
        int n = atoi(argv[2]);
        for (unsigned i = 0; i < n; i++) {
            printf("Hello %s!\n", argv[1]);
        }
    } else {
        printf("Hello %s!\n", argv[1]);
    }
    return 0;
}
