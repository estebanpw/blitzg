#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"
#include "common.h"

unsigned int tbytes = 0;


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
	
	ramUsage(0);
	
	return root;
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
	
	//char RESULT[32];
	//showWord(word, RESULT, 32);
	//printf("WORD	%s\n", RESULT);
	//getchar();
	ramUsage(0);
	
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
	
	//printf("Inserted %c\n", bitsToChar(letter));
	//printf("Inserted %c\n", bitsToChar(aux->b));
	//getchar();
	ramUsage(1);
	
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
	
	
	int searchRes = binarySearchNodes(word, root, &parent);
	int newType;
	
	
	newType = getTypeOfNode(parent);
	
	if(searchRes == 0){
		//Exact match exist, should be added to Positions table 
		//printf("Exact match!!!\n");
		//getchar();
		//TODO
		return 0;
	}else if(searchRes < 0){ //Node has to be inserted as left son of newNode
		//printf("Inserted left\n");
		//getchar();
		if(isOverlapping == 1){
			//An overlapping node [case NS, case SS]
			if(newType == 0) ((Node_N *) parent)->left = insertNode_S(getLastChar(word)); else ((Node_S *) parent)->left = insertNode_S(getLastChar(word));
			
		}else{
			//A non overlapping node [case NN, case SN]
			if(newType == 0) ((Node_N *) parent)->left = insertNode_N(word); else ((Node_S *) parent)->left = insertNode_N(word);
		}
		
		return -1;
	}else if(searchRes > 0){ //Node has to be inserted as right son of newNode
		//printf("Inserted to the right\n");
		//getchar();
		if(isOverlapping == 1){
			//An overlapping node [case NS, case SS]
			if(newType == 0) ((Node_N *) parent)->right = insertNode_S(getLastChar(word)); else ((Node_S *) parent)->right = insertNode_S(getLastChar(word));
		}else{
			//A non overlapping node [case NN, case SN]
			if(newType == 0) ((Node_N *) parent)->right = insertNode_N(word); else ((Node_S *) parent)->right = insertNode_N(word);
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
	
	
	
	int queryComp;
	unsigned char currWord[BYTES_IN_WORD], strange[BYTES_IN_WORD]; //Why???
	memset(strange, 0, BYTES_IN_WORD);
	
	char RESULT[32];
	
	
	
	while(current != NULL){
		//printf("Is null current? :  %d\n", (current == NULL));
		//Cast to correct type of Node
		if(getTypeOfNode(current) == 0){ //Type Non-overlapping
			//Cast and copy word into temp array
			castNCurr = (Node_N *) current;
			//The word is copied new because it is a non overlapping node that will hold a complete type
			memcpy(currWord, castNCurr->b, BYTES_IN_WORD);
			
			//Comparison
			
			//showWord(query, RESULT, 32);
			//printf("Got 	%s\n", RESULT); 
			//showWord(castNCurr->b, RESULT, 32);
			//printf("Comp to	%s\n", RESULT);
			queryComp = wordcmp(query, castNCurr->b, 32);
			//printf("Comparison: %d++++++++++++\n", queryComp);
			//getchar();
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
			shift_word_left(currWord);
			//printf("the shift: %c\n", bitsToChar(castSCurr->b));
			addNucleotideToWord(currWord, 'f', bitsToChar(castSCurr->b));
			
			
			//showWord(query, RESULT, 32);
			//printf("Got 	%s\n", RESULT); 
			//showWord(currWord, RESULT, 32);
			//printf("Comp to	%s\n", RESULT);
			
			//Comparison
			queryComp = wordcmp(query, currWord, 32);
			
			//printf("Comparison: %d-------------\n", queryComp);
			//getchar();
			
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
	//printf("Value of queryComp: %d\n", queryComp);
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
	//getchar();
}

/*
	Prints an overlapped node
*/

void showNode_S(const Node_S * node){
	fprintf(stdout, "NODE_S:->:%p\n	Word::%c\n	Look-up::%"PRIu32"\n", node, bitsToChar(node->b), node->n_ltable);
	//getchar();
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
		if(castNCurr->left != NULL) preOrderTraverse((void *)castNCurr->left);
		if(castNCurr->right != NULL) preOrderTraverse((void *)castNCurr->right);
	}else{
		castSCurr = (Node_S *) n;
		showNode_S(castSCurr);
		if(castSCurr->left != NULL) preOrderTraverse((void *)castSCurr->left);
		if(castSCurr->right != NULL) preOrderTraverse((void *)castSCurr->right);
	}
}


/*
	Calculates the RAM usage by the tree
	
*/

void ramUsage(int typeOfNode){
	if(typeOfNode == 0) tbytes += sizeof(Node_N);
	if(typeOfNode == 1) tbytes += sizeof(Node_S);
	//printing
	if(typeOfNode == -1) {
		printf("Current RAM usage: 	%d bytes - %d kilobytes - %d megabytes\n", tbytes, tbytes/1024, tbytes/(1024*1024));
	}
	
}







