#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "common.h"

#define KSIZE 32

int main(int argc, char ** av){
	
	//Database to read kmers from
	FILE * database;
	
	//First node of the tree
	Node_N * root;
	
	printf("Size of Node_N: %d\nSize of Node_S: %d \n", sizeof(Node_N), sizeof(Node_S));
	char * headOfMem = oneTimeMalloc(2000, KSIZE); //Use headOfMem to deallocate
	char * memPointer = headOfMem;
	
	//Open database
	database = fopen64(av[1], "rt");
	if(database == NULL) terror("Could not open database");
	
	//Variables to read kmers
	char c = 'N'; //Char to read characters

	unsigned char b[8];//, M[8];
	memset(b, 0, BYTES_IN_WORD);
	//memset(M, 0, BYTES_IN_WORD);
	
	//Variables to account for positions
	int firstTime, i, totalSeqs = 0, isOverlappingNode = 0, KPOS = 0;
	uint64_t pos = 0;
	
	unsigned char RESULT[32];
	
	while(!feof(database)){
		//Skip until finding sequence identifier
		while(!feof(database) && c != '>') c = fgetc(database);
		firstTime = 0;
		//Skip sequence identifier
		while(!feof(database) && c != '\n') c = fgetc(database);
		//Start of k-mer
		while(!feof(database) && c != '>'){
			if(firstTime == 0){ //First kmer of sequence
				while(!feof(database) && firstTime < KSIZE){ //Reading the first lot of chars
					c = fgetc(database);
					//Copy characters to current and switch mer
					if(c >= 'A' && c <= 'Z'){
						//Compress word into 8 bytes
						shift_word_left(b);
						addNucleotideToWord(b, 'f', c);	
						firstTime++;
						pos++;
					} 
				}
				KPOS = 0; //Reset for non overlapping node
			}else{
				//We already had the first kmer
				c = fgetc(database);
				while(!feof(database) && !( c >= 'A' && c <= 'Z')) c = fgetc(database); //Read until next good character
				KPOS++; //To tell when the node is overlapping or not
				pos++;
				shift_word_left(b);
				addNucleotideToWord(b, 'f', c);
				
			}
			
			
			
			
			
			if(totalSeqs == 0){ //If its the first sequence -> first node
				root = createTree(b, &memPointer);
				totalSeqs++; // First node created
			}else{
				//We will have an overlapping node 
				isOverlappingNode = (KPOS != KSIZE) ? 1 : 0;
				//printf("Sending overlapping: %d\n", isOverlappingNode);
				lookForWordAndInsert(b, root, isOverlappingNode, &memPointer);
				//If the
				if(isOverlappingNode == 0) KPOS = -1;
			}
		}
		fprintf(stdout, "Sequence of length %"PRIu64" has %"PRIu64" mers of size k=%d\n", pos, pos-KSIZE+1, KSIZE);
		
	}
	
	//preOrderTraverse(root);
	ramUsage(-1);
	fclose(database);
	
	
	free(headOfMem);
	
	/*
	unsigned char kmer[32] = "CCCCCCCCTTTTTTTTCCCCCCCCGGGGGGGG";
	
	for(i=0;i<32;i++){
		shift_word_left(b);
		addNucleotideToWord(b, 'f', kmer[i]);
	}
	
	printf("memPointer at start %p\n", memPointer);
	root = createTree(b, &memPointer);
	
	shift_word_left(b);
	addNucleotideToWord(b, 'f', 'C');
	
	printf("after root Pointer at start %p\n", memPointer);
	Node_N * prueba = insertNode_N(b, &memPointer);
	printf("after extra memPointer at start %p\n", memPointer);
	
	shift_word_left(b);
	addNucleotideToWord(b, 'f', 'A');
	
	Node_N * prueba2 = insertNode_N(b, &memPointer);
	printf("aftersecond extra memPointer at start %p\n", memPointer);
	
	showNode_N(root);
	showNode_N(prueba);
	showNode_N(prueba2);
	
	*/
	
	
	return 0;
}

