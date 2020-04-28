#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_FIFONAME_SIZE 25

struct nums {
    int n1, n2, n;
    char op;
};
typedef struct nums struct_nums;

int openAnsFIFO(int n) {
    int fd;
    char * fifoname = malloc(MAX_FIFONAME_SIZE);
    sprintf(fifoname, "tmp/fifo_ans_%d", n);
    do {
        fd = open(fifoname, O_WRONLY);
        if (fd == -1) sleep(1);
    } while (fd == -1);
    free(fifoname);
    return fd;
}

void * sum(void * arg) {
    struct_nums ns = * (struct_nums *) arg;
    int res = ns.n1 + ns.n2;
    int fd = openAnsFIFO(ns.n);
    write(fd, &res, sizeof(res));
    close(fd);
    free(arg);
    pthread_exit(NULL);
}

void * subtract(void * arg) {
    struct_nums ns = * (struct_nums *) arg;
    int res = ns.n1 - ns.n2;
    int fd = openAnsFIFO(ns.n);
    write(fd, &res, sizeof(res));
    close(fd);
    free(arg);
    pthread_exit(NULL);
}

void * multiply(void * arg) {
    struct_nums ns = * (struct_nums *) arg;
    int res = ns.n1 * ns.n2;
    int fd = openAnsFIFO(ns.n);
    write(fd, &res, sizeof(res));
    close(fd);
    free(arg);
    pthread_exit(NULL);
}

void * divide(void * arg) {
    struct_nums ns = * (struct_nums *) arg;
    int res = ns.n1 / ns.n2;
    int fd = openAnsFIFO(ns.n);
    write(fd, &res, sizeof(res));
    close(fd);
    free(arg);
    pthread_exit(NULL);
}


int main() {    
    // receive requests and create threads
    int fd, n;

    if (mkfifo("tmp/fifo_req", 0660) == -1) {
        perror("Error creating fifo_req");
        exit(EXIT_FAILURE);
    }
    fd = open("tmp/fifo_req", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Error opening fifo_req");
        exit(EXIT_FAILURE);
    }

    write(STDERR_FILENO, "[server] just made fifo_req and opened it\n", 42);

    struct_nums ns;
    while ((n = read(fd, &ns, sizeof(ns))) != -1) {
        //printf("[server] Tryed to read fifo, n = %d\n", n);
        if (n == sizeof(ns)) {
            printf("[server] received info: %d, %d\n", ns.n1, ns.n2);
            if (ns.n1 == 0 && ns.n2 == 0) break;
            pthread_t tid;
            struct_nums * ns1 = malloc(sizeof(struct_nums));
            *ns1 = ns;
            if (ns1->op == '+') pthread_create(&tid, NULL, sum, ns1);
            if (ns1->op == '-') pthread_create(&tid, NULL, subtract, ns1);
            if (ns1->op == '/') pthread_create(&tid, NULL, divide, ns1);
            if (ns1->op == '*') pthread_create(&tid, NULL, multiply, ns1);
        }
        sleep(1);
    }

    close(fd);
    unlink("tmp/fifo_req");

    pthread_exit(EXIT_SUCCESS);
}


