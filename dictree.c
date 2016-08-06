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
	
	addHeapLevel(&bpt, UINT32_MAX, 1);
	char * basePosMem = allocMemoryForPositions(totalSize); //heap for positions list
	
	
	//Variables to read kmers
	char c = 'N'; //Char to read characters

	unsigned char b[8], br[8];
	memset(b, 0, BYTES_IN_WORD);
	memset(br, 0, BYTES_IN_WORD);
	
	//Variables to account for positions
	int strandF = 1, strandR = 0;
	uint32_t pos = 0, totalSeqs = 0, crrSeqL = 0; //Up to 500*10^6 mers, should be enough
	
	//Print info
	fprintf(stdout, "[INFO] Computing tree of mers\n");
	
	c = fgetc(database);
	while(!feof(database)){
        // Check if it's a special line
        if (!isupper(toupper(c))) { // Comment, empty or quality (+) line
            if (c == '>') { // Comment line
                c = fgetc(database);
                while (c != '\n') c = fgetc(database); //Avoid comment line

                //temp.loc.seq++; // New sequence
                crrSeqL = 0; // Reset buffered sequence length

                pos++; // Absolute coordinates: add one for the "*"
            }
            c = fgetc(database); // First char of next sequence
            continue;
        }
        
        if (strandF) shift_word_left(b); // Shift bits sequence
        if (strandR) shift_word_right(br); // Shift bits sequence

        // Add new nucleotide
        switch (c) {
            case 'A': // A = 00
                crrSeqL++;
                if (strandR) br[0] |= 192;
                break;
            case 'C': // C = 01
                if (strandF) b[BYTES_IN_WORD - 1] |= 1;
                if (strandR) br[0] |= 128;
                crrSeqL++;
                break;
            case 'G': // G = 10
                if (strandF) b[BYTES_IN_WORD - 1] |= 2;
                if (strandR) br[0] |= 64;
                crrSeqL++;
                break;
            case 'T': // T = 11
                if (strandF) b[BYTES_IN_WORD - 1] |= 3;
                crrSeqL++;
                break;
            default : // Bad formed sequence
                crrSeqL = 0;
                break;
        }
        pos++;
        if (crrSeqL >= (uint64_t) KSIZE) { // Full well formed sequence
            if (strandF) {
				if(totalSeqs == 0){ //If its the first sequence -> first node
                        root = createTree(b, &bpt, basePosMem);
                        totalSeqs++; // First node created
                }else{
                	lookForWordAndInsert(b, root, &bpt, basePosMem, pos - KSIZE);
                }
            }
            /*
            if (strandR) {
                rev_temp.loc.pos = seqPos - 2; // Take position on read
                rev_temp.loc.seq = temp.loc.seq;

            }
            */
        }
		c = fgetc(database);

	}
	fprintf(stdout, "[INFO] Sequence of length %"PRIu64" has %"PRIu64" mers of size k=%d\n", pos, pos-KSIZE, KSIZE);
	
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
