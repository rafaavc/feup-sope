#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 10

void * threadFunc(void * arg) {
    sleep(1);
    printf("My TID: %lu\nn = %d\n", pthread_self(), *(int *)arg);
    pthread_exit(arg);
}


int main() {
    pthread_t thread[NUM_THREADS];
    int * thread_ret[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        int * n = malloc(sizeof(int));
        *n = i;
        pthread_create(&thread[i], NULL, threadFunc, (void *) n);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i], (void *) &thread_ret[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("%d\n", *thread_ret[i]);
    }

    pthread_exit(EXIT_SUCCESS);
}

