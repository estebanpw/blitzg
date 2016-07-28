#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"
#include "common.h"

/*
	Creates a tree with only a root node without word yet and whose position in the lookup table of repetitions is 0
	@root:	A non overlapping node that will become the root of the tree
*/

void createTree(Node_N ** root){

	*root = (Node_N *) malloc(sizeof(Node_N));
	if(root == NULL) terror("Could not allocate root node");
	(*root)->n_ltable = 0;
	(*root)->left = NULL;
	(*root)->right = NULL;
	(*root)->type = 0;
}

/*
	Links a left or right node to a parent node
	@son:	The node (either Node_N or Node_S) to be linked to a parent
	@parent:	The parent node to which the son will be linked
	@LoR:	Whether the node to be linked is a left or right leave. Can be 'l' for left and 'r' for right
	@_case:	An integer representing the possible combination of void pointers 1:[NN], 2:[NS], 3:[SN], 4:[SS] where N is a Node_N and S is a Node_S
*/

void linkNode(void * son, void * parent, const char LoR, const int _case){
	
	switch(_case){

		case 1: {
			if(LoR == 'l'){
				((Node_N *) parent)->left = (Node_N *) son;	
			}else{
				((Node_N *) parent)->right = (Node_N *) son;
			}
			break;
		}
		
			
		case 2: {
			if(LoR == 'l'){
				((Node_N *) parent)->left = (Node_S *) son;	
			}else{
				((Node_N *) parent)->right = (Node_S *) son;
			}
			break;
		}
		case 3: {
			if(LoR == 'l'){
				((Node_S *) parent)->left = (Node_N *) son;	
			}else{
				((Node_S *) parent)->right = (Node_N *) son;
			}
			break;
		}
		case 4: {
			if(LoR == 'l'){
				((Node_S *) parent)->left = (Node_S *) son;	
			}else{
				((Node_S *) parent)->right = (Node_S *) son;
			}
			break;
		}
	}
	
}

/*

*/

int lookForWordAndInsert(const char * word, Node_N * root, int isOverlapping){
	void * newNode;
	unsigned int searchRes = binarySearchNodes(word, root, &newNode), newType;
	newType = getTypeOfNode(newNode);
	
	if(searchRes == 0){
		//Exact match exist, should be added to Positions table 
		//TODO
		return 0;
	}else if(searchRes < 0){ //Node has to be inserted as left son of newNode
		if(isOverlapping == 0){ 
			//A non overlapping node
			if(newType == 0){
				//case NN 
				Node_N * aux = (Node_N *) malloc(sizeof(Node_N));
				aux->n_ltable = 0; //TODO
				memcpy(aux->b, word, BYTES_IN_WORD);
				aux->left = NULL;
				aux->right = NULL;
				aux->type = 0;
				((Node_N *) newNode)->left = aux;
			}else{
				//case NS
				Node_S * aux = (Node_S *) malloc(sizeof(Node_S));
				aux->n_ltable = 0; //TODO
				aux->b = word[strlen(word)-1];
				aux->left = NULL;
				aux->right = NULL;
				aux->type = 0;
				((Node_N *) newNode)->left = aux;
			}
		}else{
			//An overlapping node
			if(newType == 0){
				//case SN
				((Node_S *) newNode)->left = (Node_N *) malloc(sizeof(Node_N));
			}else{
				//case SS
				((Node_S *) newNode)->left = (Node_S *) malloc(sizeof(Node_S));
			}
		}
		return -1;
	}else if(searchRes > 0){ //Node has to be inserted as right son of newNode
		if(isOverlapping == 0){ 
			//A non overlapping node
			if(newType == 0){
				//case NN 
				
			}else{
				//case NS
				
		}else{
			//An overlapping node
			if(newType == 0){
				//case SN
				((Node_S *) newNode)->right = (Node_N *) malloc(sizeof(Node_N));
			}else{
				//case SS
				((Node_S *) newNode)->right = (Node_S *) malloc(sizeof(Node_S));
			}
		}
		return 1;
	}
}


/*
	Function to traverse the tree looking for an exact match comparing the word
	@query:	The compressed word we are looking for
	@root:	The root node of the tree
	@found: A node that will point to either the parent (if it has to be added) or the exact match
	
	Returns:	-1	if the match does not exist and therefore has to be added (use node "found"). -1 indicates it should be added to left son
				0	if the match exists and does not need to be added, the Node "found" will point to it
				1	if the match does not exist and therefore has to be added (use node "found"). 1 indicates it should be added to right son
*/

int binarySearchNodes(const char * query, Node_N * root, void ** found){
	void * current = root;
	Node_N * castNCurr;
	Node_S * castSCurr;
	*found = NULL;
	unsigned int haveMatch = 0, queryComp, auxPos;
	unsigned char currWord[BYTES_IN_WORD], queryWord[BYTES_IN_WORD], auxWord[BYTES_IN_WORD], mixWord[BYTES_IN_WORD];
	memcpy(queryWord, query, BYTES_IN_WORD);
	
	while(current != NULL && haveMatch == 0){
		
		//Cast to correct type of Node
		if(getTypeOfNode(current) == 0){ //Type Non-overlapping
			//Cast and copy word into temp array
			castNCurr = (Node_N *) current;
			//The word is copied new because it is a non overlapping node that will hold a complete type
			memcpy(currWord, castNCurr->b, BYTES_IN_WORD);
			auxPos = 0; //Position for the overlapping nodes in the auxiliary word
			
			//Comparison
			queryComp = wordcmp(queryWord, castNCurr->b, BYTES_IN_WORD);
			
			if(queryComp == 0){ //Exact match
				*found = (void *) castNCurr;
				return 0;
			}else if(queryComp < 0){ //The word comes before, so go for left son
				*found = (void *) castNCurr;
				current = (void *) castNCurr->left;
			}else{ //The word comes after, so go for right son
				*found = (void *) castNCurr;
				current = (void *) castNCurr->right;
			}
				
		}else{ //Type overlapping node. Remember that currWord currently holds the last non overlapping node 
			castSCurr = (Node_S *) current;	
			
			//We just have to add one byte to auxWord
			auxWord[auxPos++] = castSCurr->b;
			auxWord[auxPos] = '\0';
			//Create the real current word by mixing currWord and auxWord
			strncpy(mixWord, currWord+auxPos, 32-auxPos); //Copy right side of the currWord
			strncpy(mixWord+auxPos+1, auxWord, auxPos); //Copy the left side of the newly-added chars word
			
			//Comparison
			queryComp = wordcmp(queryWord, mixWord, BYTES_IN_WORD);
			
			if(queryComp == 0){ //Exact match
				*found = (void *) castSCurr;
				return 0;
			}else if(queryComp < 0){ //The word comes before, so go for left son
				*found = (void *) castSCurr;
				current = (void *) castSCurr->left;
			}else{ //The word comes after, so go for right son
				*found = (void *) castSCurr;
				current = (void *) castSCurr->right;
			}
		} 
	}
	//Return queryComp to tell whether the new node should be added to left or right son. The node "found" points to the last node
	return queryComp;
}

/*
	Returns the type of node, whether a 0 for a Node_N and 1 for Node_S
	@node_x:	The unknown node whose type will be checked

	Returns:	0	If node_x is a Node_N
				1	If node_x is a Node_S
*/

inline unsigned int getTypeOfNode(void * node_x){
	return ((unsigned char *) node_x)[0];
}


/*
	Adds a word properly shifted to a non overlapping node
	@node:	The non overlapping node to copy the word to
	@word	The compressed word
*/

void addWordToN_Node(Node_N * node, const unsigned char * word){
	memcpy(node->b, word, BYTES_IN_WORD);
	if(node->b == NULL) terror("Error copying word to node");
}

/*
	Adds a 2-bits character to an overlapping word
	@node:	The overlapping node to copy the word to
	@word	The compressed character
*/

void addWordToS_Node(Node_S * node, const unsigned char b){
	node->b = b;
}

/*
	Prints a non overlapping node
*/

void showNode_N(const Node_N * node){
	char kmer[32];
	showWord(node->b, kmer, 32);
	fprintf(stdout, "NODE:->:%p\n	Word::%s\n	Look-up::%"PRIu32"\n", node, kmer, node->n_ltable);
}
