#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "common.h"


int main(int argc, char ** av){
	
	if(argc != 2) terror("USE: dictree db");
	
	
	
	//Database to read kmers from
	FILE * database;
	
	//First node of the tree
	Node_N * root;
	
	//Open database
	database = fopen64(av[1], "rt");
	if(database == NULL) terror("Could not open database");
	
	//Get size of db
	fseeko64(database, 0L, SEEK_END);
	uint64_t totalSize = ftello64(database);
	fseeko64(database, 0L, SEEK_SET);
	
	//One-time heap allocation
	char * headOfMem = oneTimeMalloc(totalSize, KSIZE); //Use headOfMem to deallocate
	char * memPointer = headOfMem;
	char * basePosMem = allocMemoryForPositions(totalSize); //heap for positions list
	printf("Estimated size of database: %"PRIu64"\n", totalSize);
	printf("Size of Node_N: %d\nSize of Node_S: %d \n", sizeof(Node_N), sizeof(Node_S));
	printf("Nodes heap starts at %p\nPositons heap starts at %p\n", headOfMem, basePosMem);
	
	
	//Variables to read kmers
	char c = 'N'; //Char to read characters

	unsigned char b[8];//, M[8];
	memset(b, 0, BYTES_IN_WORD);
	//memset(M, 0, BYTES_IN_WORD);
	
	//Variables to account for positions
	int firstTime, i, totalSeqs = 0, isOverlappingNode = 0, KPOS = 0;
	uint32_t pos = 0; //Up to 500*10^6 mers, should be enough
	
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
				root = createTree(b, &memPointer, basePosMem);
				totalSeqs++; // First node created
			}else{
				//We will have a non overlapping node when KPOS equals the kmer size (i.e. every k nodes)
				isOverlappingNode = (KPOS != KSIZE) ? 1 : 0;
				lookForWordAndInsert(b, root, isOverlappingNode, &memPointer, basePosMem, pos - KSIZE);
				//If we just had a non overlapping node, reset KPOS to zero to have another k overlapping nodes
				if(isOverlappingNode == 0) KPOS = -1;
			}
		}
		fprintf(stdout, "Sequence of length %"PRIu64" has %"PRIu64" mers of size k=%d\n", pos, pos-KSIZE, KSIZE);
		
	}
	
	//preOrderTraverse(root);
	ramUsage(-1);
	fclose(database);
	
	
	//traverseTreeAndPositions((void *) root, basePosMem);
	
	free(headOfMem);
	free(basePosMem);
	/*
	unsigned char RESULT[32];
	unsigned char kmer[32] = "CCCCCCCCTTTTTTTTCCCCCCCCGGGGGGGG";
	
	for(i=0;i<32;i++){
		shift_word_left(b);
		addNucleotideToWord(b, 'f', kmer[i]);
	}
	
	printf("memPointer at start %p\n", memPointer);
	root = createTree(b, &memPointer, basePosMem);
	
	printf("Root nltable: %"PRIu32"\n", root->n_ltable);
	printf("Root position appearence is %"PRIu32"\n", getPosOfNode(root->n_ltable, basePosMem));
	
	
	
	
	linkNewPos((void *) root, basePosMem, 1500);
	linkNewPos((void *) root, basePosMem, 32000);
	
	//traversePosLists((void *)root, basePosMem);	
	
	
	
	shift_word_left(b);
	addNucleotideToWord(b, 'f', 'C');
	
	Node_N * prueba = insertNode_N(b, &memPointer, basePosMem, 32);
	printf("Prueba position appearence is %"PRIu32"\n", getPosOfNode(prueba->n_ltable, basePosMem));
	
	linkNewPos((void *) prueba, basePosMem, 1234);
	
	
	Node_N * otro = insertNode_N(b, &memPointer, basePosMem, 99);
	
	
	linkNewPos((void *) prueba, basePosMem, 666666);
	
	traversePosLists((void *)root, basePosMem);	
	traversePosLists((void *)prueba, basePosMem);	
	traversePosLists((void *)otro, basePosMem);
	//traversePosLists((void *)root, basePosMem);	
	
	*/
	
	/*
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
