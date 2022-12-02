#pragma once

#include <stdbool.h>
#include "filesLoader.h"
#include <semaphore.h>

typedef struct result{
        int numbOfIterations;
	double timeToCalculateSolution;
        int bestValue;
        int bestWeight;
}Result;

void ajkpa(File, Result*, Result*, sem_t*, sem_t*);

void printResult(Result*);

void resetResult(Result*);


