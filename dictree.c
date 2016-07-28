#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "common.h"

int main(){
	Node_N * root;
	createTree(&root);
	
	unsigned char kmer[32] = "AAAAAAAATTTTTTTTCCCCCCCCGGGGGGGG";
	unsigned char b[8];
	
	int i;
	for(i=0;i<32;i++){
		shift_word_left(b);
		addNucleotideToWord(b, 'f', kmer[i]);
	}
	
	
	addWordToN_Node(root, b);
	showNode_N(root);
	
	printf("type of root %d\n",getTypeOfNode((void *) root));
	
	getchar();
}

