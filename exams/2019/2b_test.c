#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

sem_t s1, s2;
int goOn = 1;

void * p1(void * data){
    while(goOn) {
        sem_wait(&s1);
        write(STDOUT_FILENO, "P1\n", 3);
        sem_post(&s2);
    }
    return NULL;
}

void * p2(void * data){
    while(goOn) {
        sem_wait(&s2);
        write(STDOUT_FILENO, "P2\n", 3);
        sem_post(&s1);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    sem_init(&s1, 0, 1);
    sem_init(&s2, 0, 0);

    pthread_create(&t1, NULL, p1, NULL);
    pthread_create(&t2, NULL, p2, NULL);

    sleep(2);
    goOn = 0;   

    return 0;
}