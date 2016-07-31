#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "structs.h"

Node_N * createTree(const char * word, char ** memPointer);
Node_N * insertNode_N(const char * word, char ** memPointer);
Node_S * insertNode_S(const char letter, char ** memPointer);
int lookForWordAndInsert(const char * word, Node_N * root, int isOverlapping, char ** memPointer);
int binarySearchNodes(const char * query, Node_N * root, void ** found);
unsigned int getTypeOfNode(void * node_x);
void addWordToN_Node(Node_N * node, const unsigned char * word);
void addWordToS_Node(Node_S * node, const unsigned char b);
void showNode_N(const Node_N * node);
void showNode_S(const Node_S * node);
void preOrderTraverse(void * n);
void ramUsage(int typeOfNode);
char * oneTimeMalloc(uint64_t seqlen, uint16_t ksize);
char * askForMem(int typeOfNode, char ** currMem);
