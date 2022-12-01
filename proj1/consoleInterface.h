#pragma once

#include <stdio.h>
#include <string.h>
#include "knapsack.h"

#define PARAMETERS_OK 0
#define PARAMETERS_INSUFICIENT 1

typedef struct resultTabble{
    int testNumb; //a) Número do teste (de 1 a 10).
    char testName[50];
    int numberOfItens; //b) Nome do teste e número de itens.
    double execTime; //c) Tempo total de execução.
    int pNumbers;//d) Número de processos usado (parâmetro m na descrição dos algoritmos).
    int bestValue;//e) Melhor valor da soma dos itens encontrado.
    int bestWeigth;//f) Valor da soma dos pesos da melhor solução.
    int numbOfIterations;//g) Número de iterações necessárias para chegar ao melhor valor encontrado.
    double timeToSolve;//h) Tempo que demorou até o programa atingir o melhor valor encontrado.
}ResultTable;

int getParameters(int, char**, char*, int*, int*);

void printTable(ResultTable*);

void addResultTable(ResultTable*, int, char*, int, double, int, Result);
