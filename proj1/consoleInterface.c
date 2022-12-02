#include "consoleInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "knapsack.h"


void printResultTable(ResultTable resultado){
	printf("| %5d | %10f | %5d | %4d | %9d | %14.0f |\n", 
	resultado.testNumb, resultado.execTime, resultado.bestValue, resultado.bestWeigth, 
	resultado.numbOfIterations, resultado.timeToSolve);

}

int getParameters(int agrc, char** agrv, char* fileName, int* pNumb, int* time){
	if(agrc < 6)return PARAMETERS_INSUFICIENT;

	strcpy(fileName, agrv[3]);
	*pNumb = atoi(agrv[4]);
	*time = atoi(agrv[5]);

	return PARAMETERS_OK;
}

void printTable(ResultTable* results, int size){
	printf("******************************\n*Resultado de %s com %d itens\n******************************\n",results[0].testName, results[0].numberOfItens);	
	printf("------------------------------------------------------------------\n");
	printf("| TESTE | TEMPO EXEC | VALOR | PESO | ITERAÇÕES | TEMPO SOL uSec |\n");
	printf("------------------------------------------------------------------\n");
	for(int i=0;i<size;i++){
		printResultTable(results[i]);
	}
	printf("----------------------------------------------------------------\n");
}

void printStats(ResultTable *table, int size){
	double avgValue = 0;
	double avgTime = 0;
	double avgIterations = 0;
	
	int optimalSolutionCount = 0;

	int optimaValue = table[0].bestValue;

	for(int i=0;i<size;i++){
		avgValue += table[i].bestValue;
		avgTime += table[i].timeToSolve;
		avgIterations += table[i].numbOfIterations;

		if(optimaValue == table[i].bestValue)optimalSolutionCount++;
		if(optimaValue < table[i].bestValue){
			optimalSolutionCount = 1;
			optimaValue = table[i].bestValue;
		}
	}
	
	avgValue /= size;
	avgTime /= size;
	avgIterations /= size;


	printf("\nNº medio de iterações %f\nNº medio de tempo %f\nValor otimo medio %f\nNº de maior soluções encontradas %d\n",
		avgIterations, avgTime, avgValue, optimalSolutionCount);
}

void addResultTable(ResultTable* table, int testNumb, char* file, int numbOfItems, double execTime, int pNumb, Result result){
	ResultTable aux;		
	aux.testNumb = testNumb; //a) Número do teste (de 1 a 10).
	strcpy(aux.testName, file);
	aux.numberOfItens = numbOfItems; //b) Nome do teste e número de itens.
	aux.execTime = execTime; //c) Tempo total de execução.
	aux.pNumbers = pNumb;//d) Número de processos usado (parâmetro m na descrição dos algoritmos).
	aux.bestValue = result.bestValue;//e) Melhor valor da soma dos itens encontrado.
	aux.bestWeigth = result.bestWeight;//f) Valor da soma dos pesos da melhor solução.
	aux.numbOfIterations = result.numbOfIterations;//g) Número de iterações necessárias para chegar ao melhor valor encontrado.
	aux.timeToSolve = result.timeToCalculateSolution;	

	table[testNumb-1] = aux;
}
