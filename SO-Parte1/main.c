#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include "filesLoader.h"
#include "knapsack.h"
#include "consoleInterface.h"


//variaveis necessarias nos tratamentos dos sinais
Result *result;
Result localResult;
int *childrenPids;
int numbOfP; 
sem_t *sem_signal;
bool sleepInterrupted;

//tratamento pelos prcessos filhos do sinal enviado pelo pai
void update_childsResults(int signal){
	localResult.numbOfIterations = result->numbOfIterations;
	localResult.timeToCalculateSolution = result->timeToCalculateSolution;
	localResult.bestValue = result->bestValue;
	localResult.bestWeight = result->bestWeight;
}

//tratamento pelo processo pai do sinal de um novo resultado otimo foi encontrado por um processo filho
void parent_handler(int signal){
	sleepInterrupted = true;

	/*for(int i=0;i<numbOfP;i++)
		sem_wait(sem_signal);*/

	for(int i=0;i<numbOfP;i++)
		kill(childrenPids[i], SIGUSR1);	

	/*for(int i=0;i<numbOfP;i++)
		sem_post(sem_signal);*/
	sem_post(sem_signal);
}

 int main(int argc, char **argv){
	//Numero de testes a fazer
	int numberOfTests = 10;
	
	//Parametros
	char fileName[50];
	int maxTime;
	
	if(getParameters(argc, argv, fileName, &numbOfP, &maxTime)==1){

		//Parametros default		
		strcpy(fileName, "testes/ex04.txt");
		numbOfP = 10;//processos
		maxTime = 10;//tempo		
		printf("ERRO nos parametros!\nUsando parametros Default: %s %d processos %d segundos\n", 
		fileName, numbOfP, maxTime);
	}
	
	//Semaforo
	sem_unlink("mymutex");
	sem_t *mutex = sem_open("mymutex", O_CREAT, 0644, 1);
	
	sem_unlink("mysem");
	sem_signal = sem_open("mysem", O_CREAT, 0644, 0);

	// Memoria partilhada
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_ANONYMOUS | MAP_SHARED;
	void *r = mmap(NULL, sizeof(Result), protection, visibility, 0, 0);
	
	result = (Result*)r;
	resetResult(result);
	
	//Sinais
	signal(SIGUSR1, update_childsResults);
	signal(SIGUSR2, parent_handler);

	
	
	//Leitura do ficheiro de testes
	File file;
	int fileCode = importFile(fileName, &file);
	if(fileCode == 1){printf("Ficheiro não encontrado!\n");return 0;}
	if(fileCode == 2){printf("Ficheiro com dados insuficientes!\n");return 0;}
	
	//Variaveis auxiliares
	ResultTable table[numberOfTests];
	int aux[numbOfP];
	childrenPids = aux;	
	struct timeval start, end, diff;
	bool isParent = true;
	int testNumb = 1;
	
	//Teste
	while(testNumb<11){
		// start timer.
		gettimeofday(&start, NULL);
	
		for(int i=0;i<numbOfP;i++){
			int aux = fork();		
			if(aux == 0){
				i = numbOfP;
				isParent = false;
			}
			else childrenPids[i] = aux;
		}		

		if(isParent){
			
			double time_remaining = (double)maxTime;	
			do{
				sleepInterrupted = false;
				sleep(time_remaining);
	
				// stop timer.
				gettimeofday(&end, NULL); 
			
				// Calculating total time taken by the program.
				timersub(&end, &start, &diff);			
				time_remaining = maxTime - (diff.tv_sec + diff.tv_usec/1000000);

			}while(sleepInterrupted);
			sem_wait(mutex);
			for(int i=0;i<numbOfP;i++)kill(childrenPids[i], SIGKILL);
			sem_post(mutex);
		}else{	
			ajkpa(file, result, &localResult, mutex, sem_signal);		
		}

		// stop timer.
		gettimeofday(&end, NULL); 
	
		// Calculating total time taken by the program.
		timersub(&end, &start, &diff);			
		double time_taken = diff.tv_sec + diff.tv_usec/1000000;

		addResultTable(table, testNumb, fileName, file.numbOfItems, time_taken, numbOfP, *result);
		resetResult(result);

		printf("Teste %d completo\n", testNumb++);

		//Dormir para dar tempo aos sinais
		sleep(1);
	}
	sem_close(mutex);
	sem_close(sem_signal);
	
	printTable(table, numberOfTests);
	printStats(table, numberOfTests);

	return 0;
}
