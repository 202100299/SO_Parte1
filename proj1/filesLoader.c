#include "filesLoader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int getNumbOfLines(char* fileName){
	int sum = 0;

	FILE* stream = fopen(fileName, "r");
	if(stream == NULL){return -1;}

	char aux[20];
	while(fgets(aux, 20, stream))sum++;

	fclose(stream);

	return sum;
}
void splitStr(char *line, char *value, char *weight){
	char str1[10];
	char str2[10];
	
	int count1 = 0;
	int count2 = 0;	

	bool spaceFound = false;
	int i = 0;
	while(line[i]!='\0'){
		if(line[i] == ' '){	
			spaceFound = true;
		}
		else if(spaceFound){
			str2[count2++] = line[i];
		}
		else{ 
			str1[count1++] = line[i];
		}
		i++;	
	}
	
	str1[count1] = '\0';
	str2[count2] = '\0';

	strcpy(value, str1);
	strcpy(weight, str2);
	
}
int importFile(char* fileName, File *ptF){
	//ptF = (File*)calloc(1, sizeof(File));

	int numberOfLines = getNumbOfLines(fileName);
	if(numberOfLines == -1)return LOAD_NO_FILE;
	if(numberOfLines < 4)return LOAD_INSUFICIENT_DATA;
	
        FILE* stream = fopen(fileName, "r");

        char lines[numberOfLines][20];
	int i = 0;
        while(fgets(lines[i++], 20, stream)){}

        fclose(stream);
	
	ptF->numbOfItems = atoi(lines[0]);
	ptF->maxWeight = atoi(lines[1]);
	ptF->bestValue = atoi(lines[numberOfLines-1]);
	ptF->values = (int*)calloc(numberOfLines-3, sizeof(int));
	ptF->weights = (int*)calloc(numberOfLines-3, sizeof(int));
	for(int i=2;i<numberOfLines-1;i++){
		
		char aux[2][10];
		splitStr(lines[i], aux[0], aux[1]);
		
		ptF->values[i-2] = atoi(aux[0]);	
	
		ptF->weights[i-2] = atoi(aux[1]);	
	}

	return LOAD_OK;
}
