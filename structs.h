#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define BYTES_IN_WORD 8
#pragma pack(push, 1)

/*
Currently they look mostly the same.
The point is that Node_S would not need a seq indicator, therefore using 15 bytes less.
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







