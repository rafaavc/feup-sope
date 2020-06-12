
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


# Exercício 5


# Exercício 6


# Exercício 7

