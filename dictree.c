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
	Node_N * rootA, * rootC, * rootG, * rootT;
	
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

	//Variables to hold the kmers
	char b[KSIZE+1], br[KSIZE+1];
	
	int strandF = 1, strandR = 0;
	
	//Variables to account for positions
	
	uint32_t pos = 0, crrSeqL = 0, idxSeqL = 0; //Up to 500*10^6 mers, should be enough
	uint32_t totalSeqsA = 0, totalSeqsC = 0, totalSeqsG = 0, totalSeqsT = 0;
	
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
				idxSeqL = 0; // Reset counter up to KSIZE
                pos++; // Absolute coordinates: add one for the "*"
            }
            c = fgetc(database); // First char of next sequence
            continue;
        }
        
        //Used in bits words
        //if (strandF) shift_word_left(b); // Shift bits sequence
        //if (strandR) shift_word_right(br); // Shift bits sequence

        // Add new nucleotide
        if(c == 'A' || c == 'C' || c == 'G' || c == 'T'){
        	b[idxSeqL++] = c;
			crrSeqL++;	
		}else{
			crrSeqL = 0;
			idxSeqL = 0;	
		}
        
        /*
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
        */
        pos++;
        if (crrSeqL >= (uint64_t) KSIZE) { // Full well formed sequence
        	
            if (strandF) {
            	if(b[0] == 'A'){
					if(totalSeqsA == 0){ //If its the first sequence -> first node
	                        rootA = createTree(b, &bpt, basePosMem);
	                        totalSeqsA++; // First node created
	                }else{
	                	lookForWordAndInsert(b, rootA, &bpt, basePosMem, pos - KSIZE - 1);
	                }
	            }else if(b[0] == 'C'){
	            	if(totalSeqsC == 0){ //If its the first sequence -> first node
	                        rootC = createTree(b, &bpt, basePosMem);
	                        totalSeqsC++; // First node created
	                }else{
	                	lookForWordAndInsert(b, rootC, &bpt, basePosMem, pos - KSIZE - 1);
	                }
	            	
				}else if(b[0] == 'G'){
					if(totalSeqsG == 0){ //If its the first sequence -> first node
	                        rootG = createTree(b, &bpt, basePosMem);
	                        totalSeqsG++; // First node created
	                }else{
	                	lookForWordAndInsert(b, rootG, &bpt, basePosMem, pos - KSIZE - 1);
	                }
					
				}else if(b[0] == 'T'){
					if(totalSeqsT == 0){ //If its the first sequence -> first node
	                        rootT = createTree(b, &bpt, basePosMem);
	                        totalSeqsT++; // First node created
	                }else{
	                	lookForWordAndInsert(b, rootT, &bpt, basePosMem, pos - KSIZE - 1);
	                }
					
				}
            }
        //Copy from second character to KSIZE to the 
        memcpy(b, &b[1], KSIZE-1);
        //Restart the index to the last character
        idxSeqL = KSIZE-1; 
        }
		c = fgetc(database);

	}
	fprintf(stdout, "[INFO] Sequence of length %"PRIu64" has %"PRIu64" mers of size k=%d\n", pos, pos-KSIZE, KSIZE);
	
	//traverseTreeAndPositions((void *) root, basePosMem);
	
	//Write dictionary
	fprintf(stdout, "[INFO] Writing dictionary to file\n");
	
	writeDictionary(rootA, basePosMem, dout, &bpt);
	writeDictionary(rootC, basePosMem, dout, &bpt);
	writeDictionary(rootG, basePosMem, dout, &bpt);
	writeDictionary(rootT, basePosMem, dout, &bpt);
	
	ramUsage(-1);
	fclose(database);
	fclose(dout);
	
	freeNodesMem(&bpt);
	free(basePosMem);
	
	
	
	
	return 0;
}
