#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "structs.h"

void createTree(Node_N ** root);
void linkNode(void * son, void * parent, const char LoR, const int _case);
int binarySearchNodes(const char * query, Node_N * root, void ** found);
unsigned int getTypeOfNode(void * node_x);
void addWordToN_Node(Node_N * node, const unsigned char * word);
void addWordToS_Node(Node_S * node, const unsigned char b);
void showNode_N(const Node_N * node);
