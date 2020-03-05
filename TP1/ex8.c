#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char**argv) {
    if (argc < 3) {
        printf("usage: ex8 n1 n2\nGenerates n2 random numbers from [0, n1]\n");
        exit(0);
    }
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    if (n2 <= 0) {
        printf("n2 should be greater than 0\n");
    }
    float normalized;
    int n;
    srand(time(NULL));
    for (unsigned i = 0; i < n2; i++) {
        normalized = (float) rand()/(float) RAND_MAX;
        n = normalized * n1;
        printf("%d | %d\n", i+1, n);
    }

    exit(0);
}