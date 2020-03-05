// PROGRAMA p6a.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_LENGTH 256

int main(int argc, char**argv) {
    if (argc < 3) {
        printf("usage: ex6 file1 file2\n");
        exit(0);
    }
    char * fname1 = argv[1];
    char * fname2 = argv[2];

    FILE *src, *dst;
    char buf[BUF_LENGTH];
    if ((src = fopen(fname1, "r")) == NULL) {
        perror(strerror(1));
        printf("%s\n", getenv("errno"));
        exit(1);
    }
    if ((dst = fopen(fname2, "w")) == NULL) {
        perror(strerror(2));
        exit(2);
    }
    while((fgets(buf, BUF_LENGTH, src)) != NULL) {
        fputs(buf, dst);
    }
    fclose(src);
    fclose(dst);

    exit(0); // zero é geralmente indicativo de "sucesso"
}