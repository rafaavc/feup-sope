
# Exercício 1

- **a** - V 
- **b** - V 
- **c** - F 
- **d** - F 
- **e** - V (por exemplo para medir tempos excessivos de execução ou para enforçar o *time quantum* do RR, etc)    
- **f** - F 
- **g** - V
- **h** - F
- **i** - F 
- **j** - F
- **k** - F
- **l** - F
- **m** - F
- **n** - V
- **o** - V
- **p** - V
- **q** - V
- **r** - V
- **s** - F
- **t** - F

# Exercício 2

## a
Não faz sentido iniciar um semáforo com um valor negativo pois o propósito de um semáforo é manter um número de recursos limitados, aceitando novos pedidos enquanto esse número for maior que zero. Quando o número chegar a zero, o semáforo apenas aceita novos pedidos quando um dos utilizadores libertar o recurso que requisitou.

## b

|                 P0                |                               P1                              |                               P2                              |
|---------------------------------|-------------------------------------------------------------|-------------------------------------------------------------|
| init(sem1, 1)<br> init(sem2, 0)<br> (...) | while (goOn) {<br>     wait(sem1);<br>     doWork1();<br>     signal(sem2);<br>} | while (goOn) {<br>     wait(sem2);<br>     doWork2();<br>     signal(sem1);<br>} |


# Exercício 3

## a

3a.pdf - 14 faltas de página

## b

#### 1
É possível constatar esse facto pois a partir de determinada altura, todos os acessos a memória originam falta de página.

#### 2 
1, 2, 3, 4, 1, 5, 4, 3, 6, 5, 1, 3, **2, 4, 6, 1, 5, 2, 4**

## c

#### 1 
Tendo em conta o princípio da localidade de referência, o thrashing acontece quando o processo é alocado um número de páginas inferior à quantidade de endereços no seu "working set" (o conjunto das referências com as quais está a trabalhar).

#### 2
Uma medida adequada para evitar esta situação é alocar páginas dinamicamente, definindo um intervalo de frequência de faltas de página que deve acontecer. 

Quando esse intervalo é excedido, é alocada uma nova página (se não houver páginas livres, poderá ser necessário selecionar um processo vítima para fazer swap out (suspender o processo e guardar o seu estado na memória secundária) e libertar páginas, que serão utilizadas para alocar outros processos), e quando o limite inferior do intervalo não é atingido, é desalocada uma página (que poderá depois ser alocada a outro processo).


# Exercício 4

## a

O inode de um ficheiro contém a metadata do ficheiro (autor, ultima data de modificação, etc), assim como um índice de blocos, que contém as referências para a memória onde o conteúdo do ficheiro se localiza. Como este índice tem um tamanho variável, também o ficheiro terá um tamanho variavel. No entanto, o que acontece, para melhorar a performance ao pesquisar por um bloco específico (comparativamente à pesquisa sequencial) ou para reduzir o espaço ocupado por uma dada tabela em memória, as referências dessa tabela ao invés de se desreferenciarem para um bloco, podem referenciar por si outra tabela, construindo assim uma árvore de tabelas de indices, melhorando a busca por um bloco específico (logarítmica vs linear).

## b

Um potencial problema é, no acesso sequencial a ficheiros muito grandes, ter de desreferenciar uma quantidade muito grande de referências para aceder a cada um dos blocos (devido à utilização de tabelas de índice em vários níveis).

# Exercício 5

```c
(...)
#define MAX_STRING 512

#define READ 0
#define WRITE 1

int main(int argc, char * argv [])  {
    if (argc != 2) {
        return 1;
    }

    char * outFilename = argv[1];
    int outFd = open(outFilename, O_CREAT|O_WRONLY|O_WRITE);

    int in[2], out[2];
    if (pipe(in) == -1) return 1;
    if (pipe(out) == -1) return 1;

    int pid = fork();

    if (pid == -1) return 1;
    else if (pid == 0) { // child
        close(in[WRITE]);
        close(out[READ]);
        dup2(in[READ], STDIN_FILENO);
        dup2(out[WRITE], STDOUT_FILENO);
        execlp("bc", "bc", "-qi", NULL);
        return 1;
    }

    // parent
    close(in[READ]);
    close(out[WRITE]);

    size_t n;

    char* line = NULL;
    close(in[READ]);
    close(out[WRITE]);
    FILE* f1 = fdopen(in[WRITE], "w");
    FILE* f2 = fdopen(out[READ], "r");

    while(getline(&line, &n, stdin) != -1){
        fprintf(f1, "%s", line);
        fflush(f1);
        getline(&line, &n, f2);
        line[strlen(line)-1] = '\0';
        write(outFd, line, strlen(line));
        write(outFd, " = ", 3);
        getline(&line, &n, f2);
        write(outFd, line, strlen(line));
    }

    free(line);
    close(outFd);
    return 0;
}
```

# Exercício 6

## a

Quem deve criar a fifo é o processo leitor (server-client). A responsabilidade é do processo leitor porque o propósito da informação escrita na fifo é esta ser lida pelo processo leitor, logo será este quem deve ter o controlo de quando começa e termina essa leitura. Os processos escritores apenas têm a responsabilidade de escrever mensagens no canal providenciado pelo leitor.

## b

```c
mkfifo(/users/tmp/srvfifo, 0000);
```

## c

A escrita na FIFO é uma operação atómica, logo é impossível que um escritor sobreponha a sua mensagem à de outro. Porém, na leitura, se ambos os leitores começarem a ler exatamente ao mesmo tempo, é possível que ambos leiam a mesma mensagem, mas muito improvável. Para garantir que isso não acontece pode-se utilizar um mecanismo de exclusão mútua, por exemplo uma mutex.

## d

#### 1

Para isso ser conseguido, o servidor pode dar unlink da fifo no momento em que quer fechar o FIFO para escrita, mas deixar o file descriptor aberto. Assim, poderá ler os restantes pedidos que já se encontravam na fila de espera, mas os clientes não conseguirão escrever mais pedidos.

```c
(...)

readRequests(); // ler os pedidos recebidos na fifo

unlink(srvfifo); // os escritores não conseguem escrever mais para a fifo

readRequests(); // ler os pedidos pendentes na queue

close(fifofd);  // fechar a fifo para leitura (fecho final)

(...)

```


#### 2

Se o fifo for aberto com O_NONBLOCK, o leitor vê que o fifo está vazio pois a chamada read() retorna -1, e o errno fica com o valor EAGAIN. 
```c
(...)

int n = read(fifofd, msg, msglen);
if (n == -1 && errno == EAGAIN) {
    // fifo está vazia
} else {
    (...)
} 

(...)

```
Se o fifo for aberto sem O_NONBLOCK, quando o fifo fica vazio, mas está aberto em algum processo para escrita, o processo bloqueia na chamada read, à espera que seja algo escrito para o fifo. Se todos os processos escritores fecharam a fifo do seu lado, então a chamada read retorna 0 (eof).

```c
(...)

int n = read(fifofd, msg, msglen); // bloqueia à espera que seja escrito algo no fifo (se já tiver conteúdo não bloqueia)
if (n == 0) { // eof
    // fifo está vazia
} else {
    (...)
} 

(...)

```

#### 3

O escritor sabe que não será possível escrever no FIFO pois a chamada write retornará erro (-1). Se se quiser saber se não será possível escrever na FIFO sem se tentar escrever, poder-se-á implementar uma região de memória partilhada onde se partilha o estado da fifo, ou um protocolo de cliente-servidor no qual o servidor notifica os clientes quando a fifo deixa de estar disponível.


# Exercício 7

## a

Se em vez da array se utilizasse uma única variável, quando a thread que recebe essa variável a fosse ler não haveria garantias de que a variável já teria ou não sido alterada para o valor da próxima thread no loop. Assim, com a utilização da array, garante-se a persistência do valor passado à thread por tempo suficiente para ser lido sem a possibilidade de se ler o valor errado.

## b

Terminaria o processo (todas as threads). pthread_exit(NULL) termina apenas a thread principal, e as outras threads continuam a correr.

## c

A thread bloqueia à espera do acesso à variável partilhada turn. No entanto, no acesso à variável turn para determinar se é a vez da thread, a thread faz espera ativa pela sua vez. Isto é, está sempre a verificar se turn é igual ao seu index.

## d

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int numThreads;
int firstTurn = 0; // The first thread to run must have thrIndex=turn=0
pthread_t * threadConds;

void * thr(void *arg)
{
    int thrIndex = *(int*)arg; // The effective indexes are 0,1,2,...
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&threadConds[thrIndex]);

    printf("%d ", thrIndex + 1); // The numbers shown are 1,2,3,...

    pthread_cond_signal(&threadConds[(thrIndex+1)%numThreads]);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    printf("Number of threads ? "); scanf("%d", &numThreads);
    int *arg = (int *) malloc(sizeof(int)*numThreads);
    pthread_t *tid = (pthread_t *) malloc(sizeof(pthread_t)*numThreads);

    threadConds = (pthread_cond_t *) calloc(numThreads, sizeof(pthread_cond_t));
    pthread_cond_signal(threadConds[firstTurn]);

    for (int i = 0; i < numThreads; i++)
    {
        arg[i] = i;
        pthread_create(&tid[i], NULL, thr, (void*)&arg[i]);
    }
    pthread_exit(NULL);
}

```

