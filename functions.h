#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "structs.h"

Node_N * createTree(const char * word, char ** memPointer, char * basePosMem);
Node_N * insertNode_N(const char * word, char ** memPointer, char * basePosMem, uint32_t pos);
Node_S * insertNode_S(const char letter, char ** memPointer, char * basePosMem, uint32_t pos);
int lookForWordAndInsert(const char * word, Node_N * root, int isOverlapping, char ** memPointer, char * basePosMem, uint32_t pos);
int binarySearchNodes(const char * query, Node_N * root, void ** found);
unsigned int getTypeOfNode(void * node_x);
void addWordToN_Node(Node_N * node, const unsigned char * word);
void addWordToS_Node(Node_S * node, const unsigned char b);
void showNode_N(const Node_N * node);
void showNode_S(const Node_S * node);
void preOrderTraverse(void * n);
void ramUsage(int typeOfNode);
char * oneTimeMalloc(uint64_t bytes, uint16_t ksize);
char * askForMem(int typeOfNode, char ** currMem);
char * allocMemoryForPositions(uint32_t totalMers);
uint32_t getPositionOffset();
void setFirstPosition(void * node, uint32_t pos, char * baseMem);
uint32_t getPosOfNode(uint32_t offsetPos, char * baseMem);
void linkNewPos(void * node, char * baseMem, uint32_t pos);
void traversePosLists(void * node, char * baseMem);
void traverseTreeAndPositions(void * n, char * baseMem);
