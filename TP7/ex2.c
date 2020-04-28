#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define STDERR 2
#define NUMITER 1000

int n = 100000;
pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;

void * thrfunc(void * arg) {
    int count = 0;
    fprintf(stderr, "Starting thread %s\n", (char *) arg);
    while (n > 0) {
        pthread_mutex_lock(&mut);
        n--;
        pthread_mutex_unlock(&mut);
        count++;
        write(STDERR,arg,1);
    }
    int * r = malloc(sizeof(int));
    *r = count;
    return (void *) r;
}

int main() {
    pthread_t ta, tb;
    void * c1, * c2;
    pthread_create(&ta, NULL, thrfunc, "1");
    pthread_create(&tb, NULL, thrfunc, "2");
    pthread_join(ta, (void *) &c1);
    pthread_join(tb, (void *) &c2);

    printf("\nthread 1: %d, thread 2: %d. total = %d\n", * (int*) c1, * (int*) c2, * (int*) c2 + * (int*) c1);
    return 0;
}