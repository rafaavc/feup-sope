#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct nums {
    int n1, n2;
};
typedef struct nums struct_nums;

void * sum(void * arg) {
    struct_nums ns = * (struct_nums *) arg;
    int res = ns.n1 + ns.n2;
    printf("%d+%d = %d\n", ns.n1, ns.n2, res);
    pthread_exit(NULL);
}

void * subtract(void * arg) {
    struct_nums ns = * (struct_nums *) arg;
    int res = ns.n1 - ns.n2;
    printf("%d-%d = %d\n", ns.n1, ns.n2, res);
    pthread_exit(NULL);
}

void * multiply(void * arg) {
    struct_nums ns = * (struct_nums *) arg;
    int res = ns.n1 * ns.n2;
    printf("%d*%d = %d\n", ns.n1, ns.n2, res);
    pthread_exit(NULL);
}

void * divide(void * arg) {
    struct_nums ns = * (struct_nums *) arg;
    float res = ns.n1 / ns.n2;
    printf("%d/%d = %f\n", ns.n1, ns.n2, res);
    pthread_exit(NULL);
}


int main() {
    int n1, n2;
    scanf("%d %d", &n1, &n2);
    struct_nums ns;
    ns.n1 = n1;
    ns.n2 = n2;

    pthread_t tid;

    pthread_create(&tid, NULL, sum, &ns);
    pthread_create(&tid, NULL, subtract, &ns);
    pthread_create(&tid, NULL, divide, &ns);
    pthread_create(&tid, NULL, multiply, &ns);

    pthread_exit(EXIT_SUCCESS);
}


