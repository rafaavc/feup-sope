
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

Sim. Se se alocar páginas dinamicamente ao processo
Define-se um intervalo de thrashing aceitável. Se o nível de thrashing for superior ao limite superior do intervalo, aloca-se uma página. Se o nível de thrashing for inferior ao limite inferior do intervalo, desaloca-se uma página. (técnica da frequência de faltas - aproximação à técnica do working set)


# Exercício 5

## a

Se o ficheiro for aberto em modo O_WRONLY, de cada vez que ele for aberto, o conteúdo já existente é perdido. Usando O_APPEND o novo conteúdo é adicionado ao conteúdo já existente.

## b

Abrir o ficheiro em RDONLY, copiar os dados. Adicionar os novos dados aos dados copiados. Abrir o ficheiro em WRONLY. Escrever os dados todos.


# Exercício 6

# Exercício 7

# Exercício 8



