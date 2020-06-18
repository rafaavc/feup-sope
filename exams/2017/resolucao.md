
# Exercício 1

- Permite a qualquer processador aceder qualquer posição de memória principal sem penalizações temporais.
- Garante que a instrução executada é atómica, permite implementar exclusão mútua.
- Permite avaliar a validade da página que se encontra em memória, se for válida pode ser usada, se não terá que se ir buscar à memória secundária.

# Exercício 2

## a

Os semáforos devem ser inicializados em memória partilhada, em modo partilhado.

**Inicialização**
```
init(sem1, 0); // P1 chega a A
init(sem2, 0); // P2 chega a B
init(sem3, 0); // P3 chega a C
```

**P1**
```
// A
signal(sem1);
signal(sem1);
wait(sem2);
wait(sem3);
```

**P2**
```
// B
signal(sem2);
signal(sem2);
wait(sem1);
wait(sem3);
```

**P3**
```
// C
signal(sem3);
signal(sem3);
wait(sem1);
wait(sem2);
```

## b

A solução anterior não dá origem a deadlocks pois não há exclusão mútua, uma das 4 condições necessárias e suficientes para ocorrer um deadlock.


# Exercício 3

Escalonamento do tipo preemptivo significa que a execução de um processo pode ser interrompida para executar um processo de maior prioridade, ou simplesmente como mecanismo do algoritmo de escalonamento.

O escalonamento preemptivo favorece os processos IO-bound, e não os CPU-bound devido às seguintes razões:

- Um processo CPU bound pode ser preemptado devido a uma interrupção de IO que se refere a outro processo, que voltará a ser executado (sendo esse processo de IO favorecido).

- Os processos que têm mais probabilidade de serem interrompidos por excesso de tempo de CPU são os CPU-bound, que têm CPU-bursts mais longos.

- Como os processos IO-bound executam CPU-bursts muito curtos, têm pouca probabilidade de ser preemptados.


# Exercício 4

## a

512 = 2^9
0000010 001111101
página 2, offset 125

0001111 001111101

## b

A ocorrência de thrashing pode ser detetada se todos, ou quase todos, os acessos a memória resultam em page fault. (se o processo demorar mais tempo a aceder a memória do que a executar)

O thrashing pode ser parado alocando mais páginas ao processo. (páginas essas que poderão ter de ser libertadas de outro processo, fazendo swap out)

## c

Sim. Se se alocar páginas dinamicamente ao processo.
Define-se um intervalo de thrashing aceitável. Se o nível de thrashing for superior ao limite superior do intervalo, aloca-se uma página. Se o nível de thrashing for inferior ao limite inferior do intervalo, desaloca-se uma página. (técnica da frequência de faltas - aproximação à técnica do working set)


# Exercício 5

## a

Se o ficheiro for aberto em modo O_WRONLY, de cada vez que ele for aberto, o conteúdo já existente é perdido. Usando O_APPEND o novo conteúdo é adicionado ao conteúdo já existente.

## b

Abrir o ficheiro em RDONLY, copiar os dados. Adicionar os novos dados aos dados copiados. Abrir o ficheiro em WRONLY. Escrever os dados todos.


# Exercício 6

## a

```
int numbits = argv[1];

execl("/home/sope/gen_bit", "/home/sope/gen_bit", NULL);

15
printf("%s\n", bits);
```

## b

Não haverá aleatoriedade porque como os pids são sequenciais, o resultado apenas dependerá do facto de o pid inicial ser par ou não.

**PID inicial par:**
- 1420, 1421, 1422, 1423, ...
- 0   , 1   , 0   , 1   , ...   <-- sequência gerada

**PID inicial ímpar:**
- 1421, 1422, 1423, 1424, ...
- 1   , 0   , 1   , 0   , ...   <-- sequência gerada

## c

Usar rand ?
srand(time(NULL));
rand();

## d

```
// antes da declaração de main:
int bit;

void handler(int signo) {
    if (signo == SIGUSR1) bit = 0;
    else if (signo == SIGUSR2) bit = 1;
}

// linha 6:
signal(SIGUSR1, handler);
signal(SIGUSR2, handler);

// linha 10-14:
else {
    wait(NULL);
    strcat(bits, bit == 0 ? "0" : 1);
}

```


# Exercício 7

## a

FIFO<br/>
Thread1 (P1)                   Thread2 (P2)<br/>
    ||         --> FIFO1 -->       ||<br/>
    ||         <-- FIFO2 <--       ||<br/>


## b

```
void * myThread(void * arg) {
    if (mkfifo("myreadfifo", 0666)) {
        perror("Error creating FIFO");
        pthread_exit(NULL);
    }
    int readFd, writeFd;
    if ((readFd = open("myreadfifo", O_RDONLY)) == -1) {
        perror("Error opening read fifo");
        unlink("myreadfifo");
        pthread_exit(NULL);
    }
    if ((writeFd = open("writefifo", O_WRONLY)) == -1) {
        perror("Error opening write fifo");
        close(readFd);
        unlink("myreadfifo");
        pthread_exit(NULL);
    }
    (...)
    // e):
    if (close(readFd) == -1) perror("Error closing readFd");
    if (close(writeFd) == -1) perror("Error closing writeFd");
    if (unlink("myreadfifo")==-1) perror("Error unlinking fifo");
    pthread_exit(NULL);
}
```

## c

``` 
struct Data {
    pid_t pid;
    tid_t tid;
};

void * myThread(void * arg) {
    (...)
    struct Data msg;
    msg.pid = getpid();
    msg.tid = pthread_self();
    if (write(writeFd, &msg, sizeof(msg)) == -1) {
        perror("Error writing to write FIFO");
        close(readFd);
        close(writeFd);
        unlink("myreadfifo");
        pthread_exit(NULL);
    }

    (...)
}
```

## d

Pode-se enviar uma mensagem com valores negativos, ou com um campo que diz se irá terminar a comunicação. Se não se quiser avisar antes de terminar, ao tentar comunicar depois de o outro thread terminar a comunicação dará erro. (read() dará 0, eof)

## e

no b)

# Exercício 8

## a

```
int receiverArgs[10];
tid_t receiverTids[10], callerTid;
for (int i = 0; i < 10; i++) {
    args[i] = i+1;
    pthread_create(&receiverTids[i], NULL, receiver, &receiverArgs[i]);
}
pthread_create(&callerTid, NULL,caller, NULL);
```

## b

Usaria dois semáforos. Um semáforo na thread receiver e outro na thread caller, que permitem que ocorra exclusão mútua no acesso a phoneNumber e numPhoneCalls.

## c

```
// variaveis globais
sem_t receiverSem, callerSem;
int phoneNumber;

// antes de criar as threads
sem_init(receiverSem, 1);
sem_init(callerSem, 0);
```

## d

```
while (numPhoneCalls < MAX_NUM_PHONE_CALLS) {
    // espera até que chegue uma chamada telefónica e "anota" o número de telefone
    int value = receiveCall();
    sem_wait(&receiverSem);
    phoneNumber = value;
    numPhoneCalls++;
    if (numPhoneCalls % 1000 == 0) {
        sem_post(&callerSem); // o caller irá depois dar post ao receiverSem quando terminar
    } else {
        sem_post(&receiverSem);
    }
}
```



