#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define BYTES_IN_WORD 8
#define KSIZE 32
#define MAX_HEAPS 256
#pragma pack(push, 1)

/*
 NEW IMPROVEMENT:
 
 use a lookup table for the powers of 4 in the hash calculation!!

*/

// not in use anymore
typedef struct node_o{
	unsigned char type; //0: Node_N, 1: Node_S
	unsigned char b;
	uint32_t n_ltable;
	void * left;
	void * right;
} Node_S;


typedef struct basepointertable{
	uint32_t lastP;
	char * table[MAX_HEAPS];
} basePtrTab;

typedef struct node_no{
	//unsigned char type; //0: Node_N, 1: Node_S
	unsigned char llevel; //the level for the left offset heap region
	unsigned char rlevel; //the level for the right offset heap region
	uint64_t hashmer; //K-mer word as hash
	uint32_t n_ltable;	//Position in the look-up table
	uint32_t left;	//left son (0 for null)
	uint32_t right;	//Right son (0 for null)
} Node_N;

typedef struct node_list{
	uint32_t pos;
	uint32_t offsetNext;
} l_item;




