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

Garante-se que certo código é executado numa secção crítica adquirinto uma mutex, e apenas libertando a mutex após o código ser executado. Se o código não for executado, mas a mutex for adquirida, então a mutex não será libertada.

## c

A prevenção de espera circular é uma das 4 condições necessárias e suficientes para ocorrer um deadlock. Se não ocorrer espera circular, então certamente não ocorre um deadlock.

# Exercício 3

Os processos penalizados, geralmente são os que têm bursts de CPU muito longos (CPU-bound). Os algoritmos de escalonamento dão prioridade aos processos que utilizam mais IO, quer seja para melhorar a fluidez do sistema perante o utilizador ou para realizar os acessos a memória, lentos por natureza, o mais rapidamente possível.


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

# 7

# 8
