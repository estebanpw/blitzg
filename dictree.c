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
						//shift_word_left(M);
						//addNucleotideToWord(M, 'f', c);
						firstTime++;
					} 
				}
				KPOS = 0; //Reset for non overlapping node
			}else{
				//We already had the first kmer
				c = fgetc(database);
				while(!feof(database) && !( c >= 'A' && c <= 'Z')) c = fgetc(database); //Read until next good character
				KPOS++; //To tell when the node is overlapping or not

				shift_word_left(b);
				addNucleotideToWord(b, 'f', c);
				
				//showWord(b, RESULT, 32);
				//printf("WORD	%s\n", RESULT);
				
				
			}
			
			
			
			
			
			if(totalSeqs == 0){ //If its the first sequence -> first node
				root = createTree(b);
				totalSeqs++; // First node created
			}else{
				//We will have an overlapping node 
				isOverlappingNode = (KPOS != KSIZE) ? 1 : 0;
				//printf("Sending overlapping: %d\n", isOverlappingNode);
				lookForWordAndInsert(b, root, isOverlappingNode);
				//If the
				if(isOverlappingNode == 0) KPOS = -1;
			}
		}
		
		
	}
	
	preOrderTraverse(root);
	
	fclose(database);
	
	
	
	
	/*
	unsigned char kmer[32] = "CCCCCCCCTTTTTTTTCCCCCCCCGGGGGGGG";
	
	for(i=0;i<32;i++){
		shift_word_left(b);
		addNucleotideToWord(b, 'f', kmer[i]);
	}
	
	shift_word_left(b);
	addNucleotideToWord(b, 'f', 'T');
	
	char RESULT[32];
	showWord(b, RESULT, 32);
	printf("WORD	%s\n", RESULT);
	shift_word_left(b);
	addNucleotideToWord(b, 'f', 'A');
	shift_word_left(b);
	
	showWord(b, RESULT, 32);
	printf("WORD	%s\n", RESULT);
	addNucleotideToWord(b, 'f', 'C');

	
	
	
	
	showWord(b, RESULT, 32);
	printf("WORD	%s\n", RESULT);
	
	
	
	
	
	
	
	
	root = createTree(b);
	
	
	showNode_N(root);
	
	
	
	fprintf(stdout, "Last: %d (should be a %c)\n", getLastChar(b), kmer[31]);
	
	printf("type of root %d\n",getTypeOfNode((void *) root));
	*/
	getchar();
	
	return 0;
}

