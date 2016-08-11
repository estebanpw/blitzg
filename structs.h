#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define BYTES_IN_WORD 8
#define KSIZE 32
#define READBUF 100000
#define MAX_HEAPS 256
#pragma pack(push, 1)

/*
	I believe I can remove the n_ltable atributte. Would it not be possible to number nodes according 
	to the maximum size and depth of the tree?
	
	ATTENTION!!!!!!!!!!!!!
	It is wrong
	instead of storing the byte letter of overlapping nodes, you have to store the number of previous shifts!!!!!!!!
	
	wrong again Esteban
	You have to store the number of the shift (5 bits) and then gear up the pointers direction as many times as the shift indicates
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
	unsigned char b[BYTES_IN_WORD]; //K-mer word
	uint32_t n_ltable;	//Position in the look-up table
	uint32_t left;	//left son (0 for null)
	uint32_t right;	//Right son (0 for null)
} Node_N;

typedef struct node_list{
	uint32_t pos;
	uint32_t offsetNext;
} l_item;




