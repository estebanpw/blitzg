#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "structs.h"

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a > _b ? _a : _b; })
 
#define min(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a < _b ? _a : _b; })

Node_N * createTree(const char * word, basePtrTab * bpt, char * basePosMem);
Node_N * insertNode_N(const char * word, basePtrTab * bpt, char * basePosMem, uint32_t pos, uint32_t * backOffset);
Node_S * insertNode_S(const char letter, char ** memPointer, char * basePosMem, uint32_t pos);
int lookForWordAndInsert(const char * word, Node_N * root, basePtrTab * bpt, char * basePosMem, uint32_t pos);
int binarySearchNodes(const unsigned char * query, Node_N * root, Node_N ** found, basePtrTab * bpt);
unsigned int getTypeOfNode(void * node_x);
void addWordToN_Node(Node_N * node, const unsigned char * word);
void addWordToS_Node(Node_S * node, const unsigned char b);
void showNode_N(const Node_N * node);
void showNode_S(const Node_S * node);
void preOrderTraverse(Node_N * n, basePtrTab * bpt);
void ramUsage(int typeOfNode);
void addHeapLevel(basePtrTab * bpt, uint64_t bytes_wished, int firstMalloc);
uint32_t getMemForNode(Node_N ** nnode, basePtrTab * bpt);
Node_N * getPointerFromOffset(basePtrTab * bpt, const uint32_t offset, const unsigned char level);
void freeNodesMem(basePtrTab * bpt);
char * oneTimeMalloc(uint64_t bytes);
char * askForMem(int typeOfNode, char ** currMem);
char * allocMemoryForPositions(uint32_t totalMers);
uint32_t getPositionOffset();
void setFirstPosition(Node_N * node, uint32_t pos, char * baseMem);
uint32_t getPosOfNode(uint32_t offsetPos, char * baseMem);
void linkNewPos(Node_N * node, char * baseMem, uint32_t pos);
void traversePosLists(Node_N * node, char * baseMem, FILE * f);
void traverseTreeAndPositions(Node_N * n, char * baseMem, basePtrTab * bpt);
void writeDictionary(Node_N * n, char * baseMem, FILE * f, basePtrTab * bpt);
