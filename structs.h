#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define BYTES_IN_WORD 8
#define KSIZE 32
#pragma pack(push, 1)

/*
	I believe I can remove the n_ltable atributte. Would it not be possible to number nodes according 
	to the maximum size and depth of the tree?
*/

typedef struct node_o{
	unsigned char type; //0: Node_N, 1: Node_S
	unsigned char b;
	uint32_t n_ltable;
	void * left;
	void * right;
} Node_S;


typedef struct node_no{
	unsigned char type; //0: Node_N, 1: Node_S
	unsigned char b[BYTES_IN_WORD]; //K-mer word
	uint32_t n_ltable;	//Position in the look-up table
	void * left;	//left son
	void * right;	//Right son
} Node_N;

typedef struct node_list{
	uint32_t pos;
	uint32_t offsetNext;
} l_item;




