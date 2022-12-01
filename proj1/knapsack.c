#include "knapsack.h"
#include "filesLoader.h"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>


int calculate(int bitsArr[], int arr[], int arrSize){
        int sum = 0;
        for(int i=0; i<arrSize; i++){
                sum += bitsArr[i]*arr[i];
        }
        return sum;
}
void toogleBit(int bitsArr[], int index){
        bitsArr[index] = 1 - bitsArr[index];
}
		

Result ajkpa(File f, Result* result, sem_t *mutex, bool* timeIsUp){
	
	struct timeval starttime,endtime;	

        int numbOfItems = f.numbOfItems;
        int maxWeight = f.maxWeight;
        int *values = f.values;
        int *weights = f.weights;
	
	int knapBag[numbOfItems];
        for(int i=0;i<numbOfItems;i++)knapBag[i]=0;

	int iteration = 1;


	Result localBestResult = {0, 0, 0, 0};
     
	time_t t;
        srand((unsigned) time(&t)*getpid());

	
	struct timeval start, end, diff;
	// start timer.
	gettimeofday(&start, NULL);

	while(!(*timeIsUp)){
                toogleBit(knapBag, rand()%numbOfItems);
                int weight = calculate(knapBag, weights, numbOfItems);
                int value = calculate(knapBag, values, numbOfItems);
                if(weight <= maxWeight && localBestResult.bestValue < value){			
			// stop timer.
    			gettimeofday(&end, NULL); 
			
			// Calculating total time taken by the program.
			timersub(&end, &start, &diff);			

			double time_taken = diff.tv_sec + diff.tv_usec/1000000;
	
			sem_wait(mutex);
			//Melhor Resultado de todos os processos
			if(result->bestValue < value){
				result->numbOfIterations = iteration;
				result->timeToCalculateSolution = time_taken;
				result->bestValue = value;
				result->bestWeight = weight;
			}
			sem_post(mutex);
			
			//Melhor Resultado deste processo
			localBestResult.numbOfIterations = iteration;
			localBestResult.timeToCalculateSolution = time_taken;
			localBestResult.bestValue = value;
			localBestResult.bestWeight = weight;
		}
                iteration++;
        }
	return localBestResult;
	
}


void printResult(Result *toPrint){
	printf("Solução: %d Iterações %f Segundos\n",toPrint->numbOfIterations, toPrint->timeToCalculateSolution);
	printf("Peso: %d\nValor: %d\n", toPrint->bestWeight, toPrint->bestValue);


}

void resetResult(Result* result){
	result->numbOfIterations = 0;
	result->timeToCalculateSolution = 0;
	result->bestValue = 0;
	result->bestWeight = 0;
}

