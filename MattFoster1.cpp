/*
Matt Foster
CS 302
Hash Table Implementation
*/

#include <iostream>

using namespace std;
int hashFunc(int, int);

int main (int argc, char* argv[]){
	ifstream myFile;
	myFile.open(argv[1]); // data file given at command line

// argv[2] test, 0 = linear, 1 = quadratic, all else = error
	if(argv[2]==0){
		// linear probing
	}
	else if(argv[2]==1){
		// quadratic probing
	}
	else{
		cout << "Invalid probing choice, please enter 0 for linear or 1 for quadratic." << endl;
		return 0;
	}

// this is a comment
	return 0;
}

int hashFunc(int data, int tableSize){
	int index;
	// do some hashing shit
	index = data % tableSize;
	return index;
}
