#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int numThreads;
int turn = 0; // The first thread to run must have thrIndex=turn=0
pthread_cond_t *conds = NULL;                               // <========

void * thr(void *arg){
    int thrIndex = *(int*)arg; // The effective indexes are 0,1,2,...
    while (1){
        pthread_mutex_lock(&mutex);
        while(thrIndex != turn)                             // <========
            pthread_cond_wait(&conds[thrIndex], &mutex);    // <========
        printf("%d ", thrIndex + 1);                        // <========
        turn = (turn + 1) % numThreads;                     // <========
        pthread_cond_signal(&conds[turn]);                  // <========
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    printf("Number of threads ? "); scanf("%d", &numThreads);
    int *arg = (int *) malloc(sizeof(int)*numThreads);
    conds = calloc(numThreads, sizeof(pthread_cond_t));     // <========
    pthread_cond_signal(&conds[turn]);                       // <========
    pthread_t *tid = (pthread_t *) malloc(sizeof(pthread_t)*numThreads);
    for (int i = 0; i < numThreads; i++){
        arg[i] = i;
        pthread_create(&tid[i], NULL, thr, (void*)&arg[i]);
    }
    pthread_exit(NULL);
}