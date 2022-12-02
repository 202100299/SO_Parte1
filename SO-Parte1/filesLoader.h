

#pragma once

#define LOAD_OK 0
#define LOAD_NO_FILE 1
#define LOAD_INSUFICIENT_DATA 2

typedef struct file{
        int numbOfItems;
        int maxWeight;
        int *values;
        int *weights;
        int bestValue;
}File;


int importFile(char*, File*);

