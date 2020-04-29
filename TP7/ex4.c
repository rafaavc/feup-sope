#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAXELEMS 10000000 // nr. max de posicoes
#define MAXTHREADS 100 // nr. max de threads
#define min(a, b) (a)<(b)?(a):(b)

int npos;
sem_t * mut; int pos=0, val=0; void * buf;  // variaveis partilhadas



int fill(/*void *nr*/) {
    int c = 0;
    while (1) {
        mut = (sem_t *) &(((int *) buf)[npos+2]);
        sem_wait(mut);
        pos = ((int*)buf)[0];
        val = ((int*)buf)[1];
        if (pos >= npos) {
            sem_post(mut);
            return c;
        }
        ((int*)buf)[pos+2] = val;
        ((int*)buf)[0]++; //pos
        ((int*)buf)[1]++; // val
        sem_post(mut);
        c++;
    }
}

void *verify() {
    int k;
    for (k=0; k<npos; k++) {
        if (((int*)buf)[k+2] != k) printf("ERROR: buf[%d] = %d\n", k, ((int*)buf)[k+2]);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int k, nthr, count[MAXTHREADS]; // array para contagens
    //pthread_t tidf[MAXTHREADS], tidv; // tids dos threads
    //int total;
    pid_t pids[MAXTHREADS];

    if (argc != 3) {
        printf("Usage: %s <nr_pos> <nr_thrs>\n",argv[0]);
        return 1;
    }
    npos = min(atoi(argv[1]), MAXELEMS); //no. efectivo de posicoes
    nthr = min(atoi(argv[2]), MAXTHREADS); //no. efectivo de threads

    // npos ints from buffer + pos int + var int + semaphore
    long unsigned memory_map_size = sizeof(int)*(npos+2) + sizeof(sem_t);


    int shmFD = shm_open("shm", O_CREAT | O_RDWR, 0600);
    ftruncate(shmFD, sizeof(int)*(npos+2) + sizeof(sem_t));

    char * shm = mmap(0, memory_map_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmFD, 0);
    buf = shm;

    sem_init((sem_t *) &(((int *) buf)[npos+2]), 1, 1);

    ((int*)buf)[0] = pos;
    ((int*)buf)[1] = val;
    mkfifo("total", 0660);
    int fifoFD = open("total", O_RDONLY | O_NONBLOCK);

    for (k=0; k<nthr; k++) { // criacao das threads 'fill'
        if ((pids[k] = fork()) == 0) {
            char * shm = mmap(0, memory_map_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmFD, 0);
            buf = shm;
            int n = fill();
            close(fifoFD);
            fifoFD = open("total", O_WRONLY | O_NONBLOCK);
            printf("process %d counted %d\n", getpid(), n);
            write(fifoFD, &n, sizeof(int));
            return 0;
        }
    }

    for (k = 0; k < nthr; k++) {
        waitpid(pids[k], &count[k], 0);
    }
    int n, total = 0;
    while(read(fifoFD, &n, sizeof(int)) != 0) {
        total += n;
    }
    printf("Total is %d\n", total);
    close(fifoFD);
    unlink("total");

    pid_t pid;
    if ((pid =fork()) == 0) {
        char * shm = mmap(0, memory_map_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmFD, 0);
        buf = shm;
        verify();
    }
    waitpid(pid, NULL, 0);

    munmap(0, memory_map_size);
    shm_unlink("shm");

    return 0;
}
