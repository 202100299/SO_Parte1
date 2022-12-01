#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consoleInterface.h"

int main(int agrc, char** agrv){
	char a[50];
	int b;
	int c;
	
	if(getParameters(agrc, agrv, a, &b, &c)==1){
		printf("ERRO");
		strcpy(a, "DEFAULT");
		b=-1;
		c=-1;
	}	
	printf("%s %d %d\n\n\n",a ,b, c);

	ResultTable table[10];

	for(int i=0;i<10;i++){
		ResultTable aux;		
		aux.testNumb = i+1; //a) Número do teste (de 1 a 10).
		strcpy(aux.testName, "Teste");
		aux.numberOfItens = 10; //b) Nome do teste e número de itens.
		aux.execTime = 10.0; //c) Tempo total de execução.
		aux.pNumbers = 10;//d) Número de processos usado (parâmetro m na descrição dos algoritmos).
		aux.bestValue = 10;//e) Melhor valor da soma dos itens encontrado.
		aux.bestWeigth = 10;//f) Valor da soma dos pesos da melhor solução.
		aux.numbOfIterations = 100000000;//g) Número de iterações necessárias para chegar ao melhor valor encontrado.
		aux.timeToSolve = 10.0;	
		
		table[i] = aux;	
	}
	printTable(table);

}
