# Exercício 1

A multiprogramação serve-se de DMA para aceder qualquer localização de memória com a mesma performance a partir de qualquer processador (em casos com multiplos processadores), ao invés de memória localizada, em que processadores diferentes (em placas diferentes) têm afinidade com secções de memória diferentes. Isto que permite que qualquer processo possa correr em qualquer processador, sem sofrer consequências devido a correr num processador que não tem afinidade com a localização de memória na qual o processo está armazenado.
A multiprogramação baseia-se em "IO bursts" e "CPU bursts", ou seja, um processo irá ter naturalmente secções em que utiliza o processador alternadas com secções em que utiliza IO, neste caso acesso a memória. O DMA permite que enquanto um processo acede à memória, outro processo ocupe o processador, sendo o objetivo ter o máximo de utilização do processador.

# Exercício 2

## a


**Thread principal**
```
(...)

// variáveis globais
PData pdata;
sem sem1;
sem sem2;

(...)

// antes da criação das threads T1 e T2
init(sem1, 1);
init(sem2, 0);

(...)
```

**T1**

```
(...)

wait(sem1);
// produz e adiciona dados
signal(sem2);

(...)
```


**T2**

```
(...)

wait(sem2);
// processa os dados
signal(sem1);

(...)
```


## b

#### 1

Uma secção crítica não é uma secção de código que tem de ser executada ininterruptamente, mas que tem de ser executada com **exclusão mútua**, ou seja, não pode ser executada concurrentemente.

#### 2

Garante-se que certo código é executado numa secção crítica adquirinto uma mutex, e apenas libertando a mutex após o código ser executado. Se o código não for executado, mas a mutex for adquirida, então a mutex não será libertada. O mesmo serve para outras implementações de estruturas de sincronização de processos.

## c

A prevenção de espera circular é uma das 4 condições necessárias e suficientes para ocorrer um deadlock. Se não ocorrer espera circular, então certamente não ocorre um deadlock.

# Exercício 3

Os processos penalizados, geralmente, são os que têm bursts de CPU muito longos (CPU-bound). Os algoritmos de escalonamento dão prioridade aos processos que utilizam mais IO, quer seja para melhorar a fluidez do sistema perante o utilizador ou para realizar os acessos a memória, lentos por natureza, o mais rapidamente possível.


# Exercício 4

## a

Para executar um processo que globalmente ocupa mais memória do que a física disponível, é necessário uma implementação de memória virtual. Uma das técnidas mais utilizadas é o "Paging", no qual a memória de um processo é dividida em frames na memória física. As frames que não estão a ser utilizadas estão guardadas na memória secundária (na swap zone). Quando o processo necessita de aceder uma page cuja frame não se encontra em memória principal, e caso não hajam frames na memória principal livres, então é necessário encontrar uma frame candidata a ser "swapped out", sendo a frame requisitada "swapped in" para a localização libertada.

## b

Um conhecimento de como os dados estruturados são armazenados em memória é útil para melhorar a performance do sistema. Por exemplo, dividir claramente o código em secções que utilizam um certo conjunto de estruturas localizado, o que permite reduzir a probabilidade de acontecer thrashing no acesso à memória. Este conhecimento também pode permitir menos erros de acessos a memória incorretos.


# 5

A relação entre o nome de um ficheiro e o bloco de dados que o compõe (inode) é feita através de um tuplo, da tabela do diretório:
    `(nomeFicheiro, enderecoINode)`
Isto permite que várias entradas de vários diretórios, apesar de possuírem nomes diferentes, se refiram ao mesmo ficheiro.


# 6

```
(...)
#define MAX_STRING 512

char destination_dir[MAX_STRING];
char filename[MAX_STRING];

int process_dir(char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    chdir(dirname);

    if (!(dir = opendir(dirname))) return 1;
    while ((entry = readdir(dir)) != NULL) {
        char path[1024];

        // ‐‐‐ BLOCO A ‐‐‐
        stat(entry->d_name, & statbuf);
        if (S_ISDIR(statbuf.st_mode)) { // se 'entry' for um diretório
            if (strcmp(entry‐>d_name, ".") == 0 || strcm(entry‐>d_name, "..") == 0)
                continue;
            int n = fork();
            if (n == -1) return 1;
            else if (n == 0) {
                process_dir(entry->d_name);
                exit(0);
            }
            waitpid(n, null, 0);
        }
        // ‐‐‐ FIM DO BLOCO A ‐‐‐

        // ‐‐‐ BLOCO B ‐‐‐
        else if (S_ISREG(statbuf.st_mode)) { // se 'entry' for um ficheiro regular
            if (strstr(entry‐>d_name, filename) != NULL) { // se o nome do ficheiro contiver filename
                int n = fork();
                if (n == -1) return 1;
                else if (n == 0) {
                    execlp("cp", "cp", entry->d_name, destination_dir);
                    exit(1);
                }
                waitpid(n, null, 0);
            }
        }
        // ‐‐‐ FIM DO BLOCO B ‐‐‐
    }
    return 0;
}

int main(int argc, char * argv[]) {
    // 1
    if (argc != 4) {
        exit(1);
    }

    // 2
    strcpy(filename, argv[2]);
    strcpy(destination_dir, argv[3]);

    // 3
    struct sigaction action;
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, SIG_IGN, NULL);

    // 4
    process_dir(argv[1]);

    exit(0);
}
```

## b
Se a instrução tivesse sido omitida, tanto o diretório . (o diretório atual) como o diretório .. (o diretório pai) seriam processados, o que não é desejável.

# 7

## a
 ```
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define READ 0
#define WRITE 1

int main(int argc, char * argv[]) {
    if (argc != 2) return 1;

    int fd[2];

    if (pipe(fd) != 0) return 1;

    int n = fork();

    if (n == -1) return 1;
    else if (n == 0) {
        close(fd[WRITE]);
        dup2(fd[READ], STDIN_FILENO);
        execlp("compress", "compress", NULL);
        exit(1);
    }

    close(fd[READ]);

    int fileFd = open(argv[1], O_RDONLY);
    full_copy(fileFd, fd[WRITE]);

    exit(0);
}
```

## b

Não faz muito sentido pois acaba tendo 2 instâncias de compressF a ler da mesma FIFO, faria mais sentido caso o compressF fosse executado separadamente dos myprogF (tipo servidor).
```
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_STRING 512

int main(int argc, char * argv[]) {
    if (argc != 2) return 1;

    struct Request req;

    createRequest(argv[1], &req);

    char * fifoName = malloc(sizeof(char)*MAX_STRING);
    sprintf(fifoName, "ans%ld", (long) getpid());

    int n = fork();

    if (n == -1) return 1;
    else if (n == 0) {
        execlp("compressF", "compressF", NULL);
        exit(1);
    }

    int reqFd;

    if (mkfifo(fifoName, 0660) != 0) return 1;

    while (reqFd = open("requests", O_WRONLY) == -1) { // Caso o fifo ainda não tenha sido criado
        sleep(1);
    }

    write(reqFd, &req, sizeof(req));

    int ansFd = open(fifoName, O_RDONLY);

    struct Reply reply;
    read(ansFd, &reply, sizeof(reply));
    
    write(STDOUT_FILENO, reply.data, strlen(reply.data));

    unlink(fifoName);

    return 0;
}

```

## c

Uma mensagem transmitida por pipes com mais do que PIPE_BUF (definido em limits.h) bytes, não é garantidamente atómica, pois como o seu tamanho excede o tamanho máximo do pipe, poderá ser intercalada com outras mensagens e assim comprometida.

# 8

## a

```
#define NB 10
int food_bits = 0;

// (...)

int main() {
    pthread_t tid_bird, tid_baby[NB];
    int args[NB];

    pthread_sem_init(&sem1, 0, 1);
    pthread_sem_init(&sem2, 0, 0);

    pthread_create(&tid_bird, NULL, bird, NULL);
    for (int i = 0; i < NB; i++) {
        args[i] = i;
        pthread_create(&tid_baby[i], NULL, baby, &args[i]);
    }
    return 0;
}
```

## b & c 

```
int finish = 0;
sem_t sem1, sem2;

// (...)

void *bird(void *arg) {
    while (finish == 0) {
        sem_wait(&sem1);

        get_food();
        food_bits = F;

        sem_post(&sem2);
    }
    return NULL;
}

void *baby(void *arg) {
    int id = *(int *)arg;
    int n_bits = 0;
    while (finish == 0) {
        sem_wait(&sem2);
        if (food_bits == 0) {
            sem_post(&sem1);
            fprintf(stderr, “I am baby %d, I have already eaten %d bits of food and I am still hungry!”, id, n_bits);
            thread_cond_signal(&cond);
        } else {
            food_bits--;
            sem_post(sem2);
            n_bits++;
        }
    }
    return NULL;
}

```

## d

```
int * res = malloc(sizeof(int));
pthread_exit((void *)res);
```
---
```
for (int i = 0; i < NB; i++) {
    args[i] = i;
    int * v;
    pthread_join(tid_baby[i], (void) &v);
    fprintf(stdout, “Baby number %d has eaten %d bits of food”, i, *v)
}
```

