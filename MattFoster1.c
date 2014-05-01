/*
Matt Foster
Assignment 5
CS 302
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct hashType{
	int prevSize;
	int size;
	float load;
	float maxLoad;
	int *array;
	bool probingOption; /*false = linear, true = quadratic*/
	bool full;
	bool insert;
	int read;
	int insertions;
	int collisions;
	int failures;
	int rehashes;
	int activeCells;
	int notFound;
	int searchFound;
	int searchCollisions;
	int searchFailures;
}myTable;

int myHashFunc(myTable*,int);
bool linearInsert(myTable*, int);
bool quadraticInsert(myTable*,int);
int search(myTable*,int);
void rehash(myTable*);
int nextPrime(int);
void printHash(myTable*);

int main(int argc, char *argv[]){
	myTable table;
	FILE* fileStream;
	char fileBuffer[50];
	int number;
	int i;

	table.prevSize = 0;
	table.size = 101;
	table.activeCells = 0;
	table.load = 0.0;
	table.full = false;
	table.insert = false;
	table.read = 0;
	table.failures = 0;
	table.insertions = 0;
	table.collisions = 0;
	table.rehashes = 0;
	table.notFound = 0;
	table.searchFound = 0;
	table.searchCollisions = 0;
	table.searchFailures = 0;
	fileStream = fopen(argv[1],"r");
	table.array = (int*)malloc(table.size*sizeof(int));

	if(*argv[2]== '0'){
		table.probingOption = false;
	}
	else{
		table.probingOption = true;
	}
	table.maxLoad = atof(argv[3]);

	for(i=0;i<table.size;i++)
		table.array[i] = 0;

	while(fgets(fileBuffer,sizeof(fileBuffer),fileStream)){
		sscanf(fileBuffer,"%d",&number);
		table.read++;
		if(!table.probingOption){
			table.insert = linearInsert(&table,number);
		}
		else{
			table.insert = quadraticInsert(&table,number);
		}
		if(table.insert){
			table.activeCells++;
			table.insertions++;
		}
		else{
			table.failures++;
		}
		table.load = table.activeCells/(float)table.size;
		if(table.load >= table.maxLoad){
			rehash(&table);
			table.rehashes++;
		}
		if(table.size == table.activeCells)
			table.full = true;
	}
	printHash(&table);

	/*search*/
	rewind(fileStream);

	while(fgets(fileBuffer,sizeof(fileBuffer),fileStream)){
		sscanf(fileBuffer,"%d",&number);

		if(!search(&table,number))
			table.notFound++;
	}
	printf("\nTried to find %d integers.\n", table.read);
	printf("Found %d integers.\n",table.searchFound);
	printf("Collisions: %d\n", table.searchCollisions);
	printf("Failures: %d\n\n", table.searchFailures);

	fclose(fileStream);
	return 0;
}

int myHashFunc(myTable* table, int number){
	return ((13*number+1)/2) % table->size;
}
bool linearInsert(myTable* table,int number){

	int index = myHashFunc(table,number);
	if(table->full)
		return false;

	while(true){

		if(table->array[index]==0){
			table->array[index] = number;
			return true;
		}
		table->collisions++;
		index++;

		if(index >= table->size)
			index%=table->size;
	}
}
bool quadraticInsert(myTable* table,int number){
	int i = 0;
	int index = myHashFunc(table,number);

	while(true){
		if(i > table->size)
			return false;

		if(table->array[index]==0){
			table->array[index] = number;
			return true;
		}
		table->collisions++;
		index = myHashFunc(table,number) + (pow(i,2)+1);

		if(index >= table->size)
			index %= table->size;
		i++;
	}
}
int search(myTable* table,int number){
	int index = myHashFunc(table,number);
	int i = index;

	while(true){
		if(number == table->array[index]){
			table->searchFound++;
			return 1;
		}
		table->searchCollisions++;

		if(!table->probingOption)	/*linear probing search*/
			index++;
		else						/*quadratic probing search*/
			index+=pow(i,2) + 1;
		if(index >= table->size)
			index %= table->size;
		if(index == i){
			table->searchFailures++;
			return 0;
		}
	}
}
void rehash(myTable* table){

	myTable newTable;
	int i;
	int j;

	table->prevSize = table->size;
	table->size = nextPrime(table->size);

	table->full=0;
	table->activeCells =0;

	newTable.collisions = table->collisions;
	newTable.full = 0;
	newTable.size = table->size;
	newTable.array = (int*)malloc(table->size*sizeof(int));

	for(i=0;i<table->size;i++){
		newTable.array[i] = 0;
	}

	for(j=0; j < table->prevSize; j++){
		if(table->array[j]==0){
		   continue;
		}
		if(table->probingOption){
			quadraticInsert(&newTable,table->array[j]);
		}
		else{
			linearInsert(&newTable,table->array[j]);
		}
		table->insertions++;
		table->activeCells++;
	}
	free(table->array);
	table->array = newTable.array;
	table->collisions = newTable.collisions;

	table->load = table->activeCells/(float)table->size;
}
int nextPrime(int number){
	int i;
	bool primeFlag = true;
	number = 2 * number + 1;

	while(true){
		for(i=2; i<= (number/2); i++){
			if(!(number%i)){
				primeFlag = false;	
				break;
			}
		}
		if(primeFlag){
			return number;
		}
		number += 2;
		primeFlag = true;
	}
}
void printHash(myTable* table){
	int i;
	for(i=0;i<table->size;i++)
		if(table->array[i])
			printf("Index %d:  %d\n",i,table->array[i]);
		else
			printf("Index %d:  Empty\n",i);

	printf("\nIntegers read: %d\n",table->read);
	printf("Input load factor to rehash: %f\n", table->maxLoad);
	if(!table->probingOption)
		printf("Linear probing selected.\n");
	else
		printf("Quadratic probing selected.\n");
	printf("Final size: %d\n", table->size);
	printf("Final number of elements in the table: %d\n", table->activeCells);
	printf("Final load factor: %f\n", table->load);
	printf("Insertions performed: %d\n", table->insertions);
	printf("Collisions: %d\n", table->collisions);
	printf("Failures: %d\n", table->failures);
}
