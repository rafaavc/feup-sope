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

void * clientThread(void * arg) {
    struct_nums * ns = (struct_nums *) arg;
    int fd;

    char * fifoname = malloc(MAX_FIFONAME_SIZE);
    sprintf(fifoname, "tmp/fifo_ans_%d", ns->n);

    //printf("fifoname: %s, %d\n", fifoname, ns->n);

    if (mkfifo(fifoname, 0660) == -1) {
        perror("Error creating fifo");
        pthread_exit(NULL);
    }
    fd = open(fifoname, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Error opening fifo");
        pthread_exit(NULL);
    }


    int fd1;
    do {
        fd1 = open("tmp/fifo_req", O_WRONLY | O_NONBLOCK);
        if (fd1 == -1) sleep(1);
    } while (fd1 == -1);

    write(fd1, ns, sizeof(struct_nums));
    close(fd1);
    if (ns->n1 == 0 && ns->n2 == 0) {
        free(ns);
        close(fd);
        unlink(fifoname);
        free(fifoname);
        pthread_exit(NULL);
    }

    int n, res;
    while((n = read(fd, &res, sizeof(res))) != -1) {
        if (n == sizeof(res)) {
            break;
        }
        sleep(1);
    }
    printf("= %d\n", res);

    free(ns);
    close(fd);
    unlink(fifoname);
    free(fifoname);
    pthread_exit(NULL);
}

int main() {
    int n1, n2, n = 0;
    char op;
    struct_nums * ns;

    pthread_t tid;

    do {
        scanf("%d%c%d", &n1, &op, &n2);
        ns = malloc(sizeof(struct_nums));
        ns->n1 = n1;
        ns->n2 = n2;
        ns->op = op;
        ns->n = n;
        n++;
        pthread_create(&tid, NULL, clientThread, ns);
    } while(ns->n1 != 0 || ns->n2 != 0);

    pthread_exit(EXIT_SUCCESS);
}