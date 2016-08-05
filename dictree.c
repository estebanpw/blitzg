#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "common.h"



int main(int argc, char ** av){
	
	if(argc != 3) terror("USE: dictree db dbout");
	
	
	
	//Database to read kmers from and output dictionary
	FILE * database, * dout;
	
	//First node of the tree
	Node_N * root;
	
	//Open database
	database = fopen64(av[1], "rt");
	if(database == NULL) terror("Could not open database");
	
	//Out database
	dout = fopen64(av[2], "wt");
	if(dout == NULL) terror("Could not open output database dictionary");
	
	//Get size of db
	fseeko64(database, 0L, SEEK_END);
	uint64_t totalSize = ftello64(database);
	fseeko64(database, 0L, SEEK_SET);
	
	//Create table to store offset pointers
	basePtrTab bpt;
	bpt.lastP = 0;
	
	
	//One-time heap allocation
	
	addHeapLevel(&bpt, UINT32_MAX/2, 1);
	char * basePosMem = allocMemoryForPositions(totalSize); //heap for positions list
	
	
	//Variables to read kmers
	char c = 'N'; //Char to read characters

	unsigned char b[8];//, M[8];
	memset(b, 0, BYTES_IN_WORD);
	//memset(M, 0, BYTES_IN_WORD);
	
	//Variables to account for positions
	int firstTime, i, totalSeqs = 0, isOverlappingNode = 0, KPOS = 0;
	uint32_t pos = 0; //Up to 500*10^6 mers, should be enough
	fprintf(stdout, "[INFO] Computing tree of mers\n");
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
			}else{
				//We already had the first kmer
				c = fgetc(database);
				while(!feof(database) && !( c >= 'A' && c <= 'Z')) c = fgetc(database); //Read until next good character
				pos++;
				shift_word_left(b);
				addNucleotideToWord(b, 'f', c);
				
			}
			
			
			
			
			
			if(totalSeqs == 0){ //If its the first sequence -> first node
				root = createTree(b, &bpt, basePosMem);
				totalSeqs++; // First node created
			}else{
				lookForWordAndInsert(b, root, &bpt, basePosMem, pos - KSIZE);
			}
		}
		fprintf(stdout, "[INFO] Sequence of length %"PRIu64" has %"PRIu64" mers of size k=%d\n", pos, pos-KSIZE, KSIZE);
		
	}
	
	
	//traverseTreeAndPositions((void *) root, basePosMem);
	
	//Write dictionary
	fprintf(stdout, "[INFO] Writing dictionary to file\n");
	

	//traverseTreeAndPositions(root, basePosMem, &bpt);
	
	//writeDictionary(root, basePosMem, dout, &bpt);
	writeDictionary(root, basePosMem, dout, &bpt);
	
	ramUsage(-1);
	fclose(database);
	fclose(dout);
	
	freeNodesMem(&bpt);
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
