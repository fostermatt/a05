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
}hashTable;

/*Prototypes*/
int search(hashTable*,int);
int nextPrime(int);
int linearInsert(hashTable*, int);
int quadraticInsert(hashTable*,int);
void printHash(hashTable*);
void rehash(hashTable*);
int myHashFunc(hashTable*,int);
/*==========================================================*/
int main(int argc, char *argv[]){
	hashTable table;
	FILE* fileStream;
	char fileBuffer[50];
	int number;
	int i;

	/*Initialize struct values*/
	table.prevSize = 0;
	table.size = 7;
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

	if(*argv[2]== '0')
		table.probingOption = false;
	else
		table.probingOption = true;
	table.maxLoad = atof(argv[3]);

	for(i=0;i<table.size;i++)
		table.array[i] = 0;

	/*Main loop, inserts elements into hash Table, keeps track of
	  of load factor as well as rehashes when necessary*/
	while(fgets(fileBuffer,sizeof(fileBuffer),fileStream)){
		sscanf(fileBuffer,"%d",&number);
		table.read++;
		if(!table.probingOption)
			table.insert = linearInsert(&table,number);
		if(table.probingOption)
			table.insert = quadraticInsert(&table,number);
		if(table.insert){
			table.activeCells++;
			table.insertions++;
		}
		if(!table.insert)
			table.failures++;

		/*Update load Factor*/		
		table.load = table.activeCells/(float)table.size;
		/*If no maxLoad was given and the insertion failed, rehash*/
		if(table.load >= table.maxLoad){
			rehash(&table);
			table.rehashes++;
			if(fgets(fileBuffer,sizeof(fileBuffer),fileStream)){
				sscanf(fileBuffer,"%d",&number);
				if(table.probingOption)
					quadraticInsert(&table,number);
				if(!table.probingOption)
					linearInsert(&table,number);
				table.activeCells++;
				table.insertions++;
			}
		}
		if(table.size == table.activeCells)
			table.full = true;
	}
	printHash(&table);
	/*Reread and search*/
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

int myHashFunc(hashTable* table, int number){
	return number%table->size;
}

int linearInsert(hashTable* table,int number){

	int index = myHashFunc(table,number);
	if(table->full)
		return 0;

	while(true){
		/*Maps to empty spot!*/
		if(table->array[index]==0){
			table->array[index] = number;
			return 1;
		}
		table->collisions++;
		index++;
		/*Should not go out of bounds*/
		if(index >= table->size)
			index%=table->size;
	}
}

/*Adds elements to hash table, if collision, uses quadratic probing to insert
 elements that offset from original mapped spot, loops around array if bounds go
 over tableSize*/
int quadraticInsert(hashTable* table,int number){
	int i = 0;
	int index = myHashFunc(table,number);

	while(true){
		/*Due to pattern of i^2%table->size it is only necessary to check up to
		  table->size/2, else it just keeps repeating*/
		if(i > table->size/2)
			return 0;
		/*Maps to empty spot!*/
		if(!table->array[index]){
			table->array[index] = number;
			return 1;
		}
		table->collisions++;
		/*Quadratic probing of perfect square 1,4,9,16,25...
		  using the fact they differ by odd number, 3,5,7,9...*/
		index += 2*i+1;
		i++;;
		/*Should not go out of bounds*/
		if(index >= table->size)
			index %= table->size;
	}
}

int search(hashTable* table,int number){
	int index = number%table->size;
	int i= index;

	while(1){
		if(number == table->array[index]){
			table->searchFound++;
			return 1;
		}
		index++;
		table->searchCollisions++;

		if(index >= table->size)
			index %= table->size;
		if(index == i){
			table->searchFailures++;
			return 0;
		}
	}
}

/*Rehashes table once it has reached a size bigger than max load*/
void rehash(hashTable* table){
	/*New table size of next prime number twice as big*/
	hashTable newTable;
	int i;
	int j;

	/*new table size*/
	table->prevSize = table->size;
	table->size = nextPrime(table->size);

	table->full=0;
	table->activeCells =0;
	/*Initialize new table */
	newTable.collisions = table->collisions;
	newTable.full = 0;
	newTable.size = table->size;
	newTable.array = (int*)malloc(table->size*sizeof(int));

	for(i=0;i<table->size;i++)
		newTable.array[i] = 0;

	/*hash every element of old table to new table*/
	for(j=0; j < table->prevSize; j++){
		/*Element is zero, do not insert*/
		if(table->array[j]==0)
		   continue;

		if(table->probingOption){
			quadraticInsert(&newTable,table->array[j]);
		}
		else{
			linearInsert(&newTable,table->array[j]);
		}

		table->insertions++;
		table->activeCells++;
	}
	/*free memory of old hashTable*/
	free(table->array);
	table->array = newTable.array;
	table->collisions = newTable.collisions;
	/*Update new load Factor*/
	table->load = table->activeCells/(float)table->size;
}

int nextPrime(int number){
	int i;
	bool primeFlag = true;
	number = 2 * number + 1;

	while(true){
		for(i=2; i<= (sqrt(number)+1); i++){
			if(!(number%i)){
				primeFlag = false;	
				break;
			}
		}
		if(primeFlag)
			return number;
		/*Check next possible prime*/
		number += 2;
		primeFlag = true;
	}
}

void printHash(hashTable* table){
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
