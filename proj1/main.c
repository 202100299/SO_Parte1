#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "filesLoader.h"
#include "knapsack.h"
#include "consoleInterface.h"


 int main(int argc, char **argv){
	


	sem_unlink("mymutex");
	sem_t *mutex = sem_open("mymutex", O_CREAT, 0644, 1);
	// Create shared memory map
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_ANONYMOUS | MAP_SHARED;
	void *r = mmap(NULL, sizeof(Result), protection, visibility, 0, 0);
	void *b = mmap(NULL, sizeof(bool), protection, visibility, 0, 0);
	
	Result *result = (Result*)r;
	bool *timesUp = (bool*)b;
	

	char fileName[50];
	int numbOfP;
	int maxTime;
	
	if(getParameters(argc, argv, fileName, &numbOfP, &maxTime)==1){

		//Parametros default		
		strcpy(fileName, "testes/ex04.txt");
		numbOfP = 10;//processos
		maxTime = 10;//tempo		
		printf("ERRO nos parametros!\nUsando parametros Default: %s %d processos %d segundos\n", 
		fileName, numbOfP, maxTime);
	}

	int childrenPids[numbOfP];
	bool isParent = true;	
	*timesUp = false;	

	File file;
	int fileCode = importFile(fileName, &file);
	if(fileCode == 1){printf("Ficheiro não encontrado!\n");return 0;}
	if(fileCode == 2){printf("Ficheiro com dados insuficientes!\n");return 0;}
	
	ResultTable table[10];

	clock_t start, end;
	
	time(&end);
	double time_taken = end - start;

	int testNumb = 1;
	while(testNumb<11){
		//inico temporizador
		time(&start);

		for(int i=0;i<numbOfP;i++){
			int aux = fork();		
			if(aux == 0){
				i = numbOfP;
				isParent = false;
			}
			else childrenPids[i] = aux;
		}
		

		if(isParent){
			sleep(maxTime);
			*timesUp = true;
			for(int i=0;i<numbOfP;i++)wait(0);
		}else{	
			Result localR = ajkpa(file, result, mutex, timesUp);		
			exit(0);
		}
		
		//fim temporizador
		time(&end);
		double time_taken = end - start;

		addResultTable(table, testNumb, fileName, file.numbOfItems, time_taken, numbOfP, *result);
		resetResult(result);

		printf("Teste %d completo\n", testNumb++);
	}
	sem_close(mutex);
	
	printTable(table);

	return 0;
}
