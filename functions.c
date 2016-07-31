#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"
#include "common.h"

/*
	Creates a tree with only a root node without word yet and whose position in the lookup table of repetitions is 0
	@root:	A non overlapping node that will become the root of the tree
	@word:	The non overlapping word that will be the root of the tree
*/

Node_N * createTree(const char * word){

	Node_N * root = (Node_N *) malloc(sizeof(Node_N));
	if(root == NULL) terror("Could not allocate root node");
	root->n_ltable = 0;
	root->left = NULL;
	root->right = NULL;
	root->type = 0;
	memcpy(root->b, word, BYTES_IN_WORD);
	return root;
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
	Creates a Node_N ready to be inserted
	@word:	The word for the Node_N
	
	Returns	a pointer to newly allocated node
*/

Node_N * insertNode_N(const char * word){
	Node_N * aux = (Node_N *) malloc(sizeof(Node_N));
	aux->n_ltable = 0; //TODO
	memcpy(aux->b, word, BYTES_IN_WORD);
	aux->left = NULL;
	aux->right = NULL;
	aux->type = 0;
	return aux;
}

/*
	Creates a Node_S ready to be inserted
	@letter:	The letter that added up to the k-1 letters from the ancesters forms a complete word
	
	Returns	a pointer to newly allocated node
*/

Node_S * insertNode_S(const char letter){
	Node_S * aux = (Node_S *) malloc(sizeof(Node_S));
	aux->n_ltable = 0; //TODO
	aux->b = letter;
	aux->left = NULL;
	aux->right = NULL;
	aux->type = 1;
	return aux;
}

/*
	Receives a word, performs binary search on the words tree, and 
	it gets inserted in case the word did not exist
	@word:	The word we are looking for
	@root:	The root node of the binary tree
	@isOverlapping:	0 if it is a Non-overlapping node
					1 if it is an overlapping node
	
	Returns	0 if there is an exact match
			1 if the node was inserted as right son
			-1 if the node was inserted as left son
*/

int lookForWordAndInsert(const char * word, Node_N * root, int isOverlapping){
	void * parent;
	
	
	unsigned int searchRes = binarySearchNodes(word, root, &parent);
	unsigned int newType;
	
	
	
	newType = getTypeOfNode(parent);
	
	if(searchRes == 0){
		//Exact match exist, should be added to Positions table 
		//TODO
		return 0;
	}else if(searchRes < 0){ //Node has to be inserted as left son of newNode
		if(isOverlapping == 0){ 
			//A non overlapping node [case NN, case NS]
			if(newType == 0) ((Node_N *) parent)->left = insertNode_N(word); else ((Node_N *) parent)->left = insertNode_S(getLastChar(word));
		}else{
			//An overlapping node [case SN, case SS]
			if(newType == 0) ((Node_S *) parent)->left = insertNode_N(word); else ((Node_S *) parent)->left = insertNode_S(getLastChar(word));
		}
		return -1;
	}else if(searchRes > 0){ //Node has to be inserted as right son of newNode
		if(isOverlapping == 0){ 
			//A non overlapping node [case NN, case NS]
			if(newType == 0) ((Node_N *) parent)->right = insertNode_N(word); else ((Node_N *) parent)->right = insertNode_S(getLastChar(word));
		}else{
			//An overlapping node [case SN, case SS]
			if(newType == 0) ((Node_S *) parent)->right = insertNode_N(word); else ((Node_S *) parent)->right = insertNode_S(getLastChar(word));
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
	
	char RESULT[32];
	showWord(query, RESULT, 32);
	printf("START	%s\n", RESULT); 
	showWord(root->b, RESULT, 32);
	printf("ROOTO	%s\n", RESULT); 
	
	
	while(current != NULL && haveMatch == 0){
		
		//Cast to correct type of Node
		if(getTypeOfNode(current) == 0){ //Type Non-overlapping
			//Cast and copy word into temp array
			castNCurr = (Node_N *) current;
			//The word is copied new because it is a non overlapping node that will hold a complete type
			memcpy(currWord, castNCurr->b, BYTES_IN_WORD);
			auxPos = 0; //Position for the overlapping nodes in the auxiliary word
			
			//Comparison
			
			showWord(queryWord, RESULT, 32);
			printf("Got 	%s\n", RESULT); 
			showWord(castNCurr->b, RESULT, 32);
			printf("Comp to	%s\n", RESULT);
			queryComp = wordcmp(queryWord, castNCurr->b, BYTES_IN_WORD);
			getchar();
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
	showWord(node->b, kmer, BYTES_IN_WORD*4);
	fprintf(stdout, "NODE_N:->:%p\n	Word::%s\n	Look-up::%"PRIu32"\n", node, kmer, node->n_ltable);
}

/*
	Prints an overlapped node
*/

void showNode_S(const Node_S * node){
	char kmer[BYTES_IN_WORD];
	char finalMer[BYTES_IN_WORD];
	kmer[31] = node->b;
	kmer[32] = '\0';
	showWord(kmer, finalMer, BYTES_IN_WORD*4);
	fprintf(stdout, "NODE_S:->:%p\n	Word::%s\n	Look-up::%"PRIu32"\n", node, finalMer, node->n_ltable);
}

/*
	Traverses a tree showing its nodes in pre-order
	@n:	The root node
*/

void preOrderTraverse(void * n){
	Node_N * castNCurr;
	Node_S * castSCurr;
	if(getTypeOfNode(n) == 0){ //Type Non-overlapping
		castNCurr = (Node_N *) n;
		showNode_N(castNCurr);
		if(castNCurr->left != NULL) preOrderTraverse(castNCurr->left);
		if(castNCurr->right != NULL) preOrderTraverse(castNCurr->right);
	}else{
		castSCurr = (Node_S *) n;
		showNode_S(castSCurr);
		if(castSCurr->left != NULL) preOrderTraverse(castSCurr->left);
		if(castSCurr->right != NULL) preOrderTraverse(castSCurr->right);
	}
	
}









