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

Node_N * createTree(const char * word, basePtrTab * bpt, char * basePosMem){

	//Node_N * root = (Node_N *) malloc(sizeof(Node_N));
	Node_N * root;
	getMemForNode(&root, bpt);
	if(root == NULL) terror("Could not allocate root node");
	root->left = 0;
	root->right = 0;
	memcpy(root->b, word, BYTES_IN_WORD);
	setFirstPosition(root, 0, basePosMem);
	ramUsage(0);
	
	return root;
}

/*
	Creates a Node_N ready to be inserted
	@word:	The word for the Node_N
	@memPointer:	Char pointer to the current position of the memory index
	@pos:	The position of the kmer itself
	@backOffset:	The returned offset from the memory manager must be stored in left or rightson
	
	Returns	a pointer to newly allocated node
*/

Node_N * insertNode_N(const char * word, basePtrTab * bpt, char * basePosMem, uint32_t pos, uint32_t * backOffset){
	//Node_N * aux = (Node_N *) malloc(sizeof(Node_N));
	//Node_N * aux = (Node_N *) askForMem(0, memPointer);
	Node_N * aux;
	*backOffset = getMemForNode(&aux, bpt); //Assign the offset to the member of the parent node
	memcpy(aux->b, word, BYTES_IN_WORD);


	aux->left = 0;
	aux->right = 0;
	//aux->type = 0;
	setFirstPosition(aux, pos, basePosMem);
	
	ramUsage(0);
	return aux;
}

/*
	Creates a Node_S ready to be inserted
	@letter:	The letter that added up to the k-1 letters from the ancesters forms a complete word
	@memPointer:	Char pointer to the current position of the memory index
	
	Returns	a pointer to newly allocated node
*/

Node_S * insertNode_S(const char letter, char ** memPointer, char * basePosMem, uint32_t pos){
	/*
	//Node_S * aux = (Node_S *) malloc(sizeof(Node_S));
	Node_S * aux = (Node_S *) askForMem(1, memPointer);
	aux->b = letter;
	aux->left = NULL;
	aux->right = NULL;
	aux->type = 1;
	setFirstPosition(aux, pos, basePosMem);
	ramUsage(1);
	
	return aux;
	*/
	return NULL;
	
}

/*
	Receives a word, performs binary search on the words tree, and 
	it gets inserted in case the word did not exist
	@word:	The word we are looking for
	@root:	The root node of the binary tree
	@isOverlapping:	0 if it is a Non-overlapping node
					1 if it is an overlapping node
	@memPointer:	A char pointer to the position in memory of the last word added
	
	Returns	0 if there is an exact match
			1 if the node was inserted as right son
			-1 if the node was inserted as left son
*/

int lookForWordAndInsert(const char * word, Node_N * root, basePtrTab * bpt, char * basePosMem, uint32_t pos){
	Node_N * parent;
	
	//printf("==============\n");
	//traverseTreeAndPositions((void *)root, basePosMem);
	//printf("==============\n");
	
	int searchRes = binarySearchNodes(word, root, &parent, bpt);
	//newType = getTypeOfNode(parent);
	
	if(searchRes == 0){
		//Exact match exist, should be added to Positions table 
		//printf("Exact match!!!@%"PRIu32"\n", pos);
		
		linkNewPos(parent, basePosMem, pos);
		ramUsage(2);
		return 0;
	}else if(searchRes < 0){ //Node has to be inserted as left son of newNode
		
		insertNode_N(word, bpt, basePosMem, pos, &parent->left);
		parent->llevel = bpt->lastP;
		/*
		//printf("Inserted left\n");
		if(isOverlapping == 1){
			//An overlapping node [case NS, case SS]
			
			if(newType == 0) ((Node_N *) parent)->left = insertNode_S(getLastChar(word), memPointer, basePosMem, pos); else ((Node_S *) parent)->left = insertNode_S(getLastChar(word), memPointer, basePosMem, pos);
			
		}else{
			//A non overlapping node [case NN, case SN]
			if(newType == 0) ((Node_N *) parent)->left = insertNode_N(word, memPointer, basePosMem, pos); else ((Node_S *) parent)->left = insertNode_N(word, memPointer, basePosMem, pos);
		}
		*/
		return -1;
	}else if(searchRes > 0){ //Node has to be inserted as right son of newNode
		//printf("Inserted to the right\n");
		
		 insertNode_N(word, bpt, basePosMem, pos, &parent->right); //Assign offset of right child to parent
		 parent->rlevel = bpt->lastP;  //store to which level does the offset correspond to
		/*
		if(isOverlapping == 1){
			//An overlapping node [case NS, case SS]
			if(newType == 0) ((Node_N *) parent)->right = insertNode_S(getLastChar(word), memPointer, basePosMem, pos); else ((Node_S *) parent)->right = insertNode_S(getLastChar(word), memPointer, basePosMem, pos);
		}else{
			//A non overlapping node [case NN, case SN]
			if(newType == 0) ((Node_N *) parent)->right = insertNode_N(word, memPointer, basePosMem, pos); else ((Node_S *) parent)->right = insertNode_N(word, memPointer, basePosMem, pos);
		}
		*/
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

int binarySearchNodes(const unsigned char * query, Node_N * root, Node_N ** found, basePtrTab * bpt){
	Node_N * current = root;
	*found = NULL;
	int stop = 0;
		
	int queryComp;
	do{
					
		//Comparison
		
		queryComp = wordcmp(query, current->b, BYTES_IN_WORD);
		
		/*
		int i;
		for(i=0;i<8;i++){
			printf("%d - %d\n", query[i], current->b[i]);
		}
		
		char kmer[32];
		showWord(query, kmer, 32);
		printf("Comparing %s to\n", kmer);
		showWord(current->b, kmer, 32);
		printf("          %s yields %d\n", kmer, queryComp);
		//showNode_N(current);
		//getchar();
		*/
		if(queryComp == 0){ //Exact match
			
			
			*found = current;
			return 0;
		}else if(queryComp < 0){ //The word comes before, so go for left son
			
			*found = current;
			if(current->left == 0) stop = 1;
			current = getPointerFromOffset(bpt, current->left, current->llevel);
		}else{ //The word comes after, so go for right son
			*found = current;
			
			if(current->right == 0) stop = 1;
			current = getPointerFromOffset(bpt, current->right, current->rlevel);
			
		}
			
	} while(stop == 0);
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
	fprintf(stdout, "NODE_N:->:%p\n	Word::%s\n	Look-up::%"PRIu32"\nL-lvl: %"PRIu32"\nL-off: %"PRIu32"\nR-lvl: %"PRIu32"\nR-off: %"PRIu32"\n", node, kmer, node->n_ltable, node->llevel, node->left, node->rlevel, node->right);
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


void preOrderTraverse(Node_N * n, basePtrTab * bpt){
	showNode_N(n);
	if(n->left != 0) preOrderTraverse(getPointerFromOffset(bpt, n->left, n->llevel), bpt);
	if(n->right != 0) preOrderTraverse(getPointerFromOffset(bpt, n->right, n->rlevel), bpt);
}


/*
	Calculates the RAM usage by the tree
	
*/

void ramUsage(int typeOfNode){
	static uint64_t tbytes = 0;
	if(typeOfNode == 0) tbytes += sizeof(Node_N) + sizeof(l_item);
	if(typeOfNode == 1) tbytes += sizeof(Node_S) + sizeof(l_item);
	if(typeOfNode == 2) tbytes += sizeof(l_item);
	//printing
	if(typeOfNode == -1) {
		printf("Current RAM usage: 	%"PRIu64" bytes - %"PRIu64" kilobytes - %"PRIu64" megabytes\n", tbytes, tbytes/1024, tbytes/(1024*1024));
	}
}

/*
	Allocates one heap of memory (up to 4 GB) and stores the base pointer in the lookup table
	@bpt:	Table of base pointers
	@bytes_wished:	Amount of bytes wished to allocate; only 4GB will be allocated at once
	@firstMalloc:	1 if its the first heap we allocated, 0 otherwise
*/

void addHeapLevel(basePtrTab * bpt, uint64_t bytes_wished, int firstMalloc){
	if(bpt->lastP + 1 > MAX_HEAPS) terror("Reached max memory allowed");
	if(firstMalloc != 1) bpt->lastP += 1;
	bpt->table[bpt->lastP] = oneTimeMalloc(min(bytes_wished, UINT32_MAX));
	printf("Allocated an initial: %"PRIu64" bytes\n", min(bytes_wished, UINT32_MAX));
}

/*
	Reserves a region of the heap memory for the new node
	@nnode:	the new node to add
	@bpt:	table of base pointers
	
	Returns the position offset within the level
*/

uint32_t getMemForNode(Node_N ** nnode, basePtrTab * bpt){
	
	static uint32_t currPosition = 0;
	
	//Check if we still have memory left on this level
	if(currPosition > (UINT32_MAX - sizeof(Node_N *))){
		//create new heap
		addHeapLevel(bpt, UINT32_MAX, 0);
		currPosition = 0;
	}
	
	//Give memory to pointer
	
	//fprintf(stdout, "POS:::::::::::::%p\n", (bpt->table[bpt->lastP]+(uint64_t)currPosition));
	//getchar();
	*nnode = (Node_N *) (bpt->table[bpt->lastP]+(uint64_t)currPosition);
	//printf("the real pos ::::::::::::%p\n", *nnode);
	//Increment position for the next one
	currPosition += sizeof(Node_N);
	
	return (currPosition - (uint32_t) sizeof(Node_N));
}


/*
	Retrieves the node on level l and position p given the base pointers table
	@bpt:	Base pointer table that holds the base pointers positions
	@offset:	Offset of the particular node
	@level:		Level in the table where the node is located
	
	Returns the pointer to the node
*/

Node_N * getPointerFromOffset(basePtrTab * bpt, const uint32_t offset, const unsigned char level){
	//add offset to pointer and return the chosen node
	//fprintf(stdout, "POS:::::::::::::%p\n", (bpt->table[level]+(uint64_t)offset));
	//getchar();
	return (Node_N *) (bpt->table[level]+(uint64_t)offset);
	
}

/*
	This functions linearly freeds all allocated pointers
	@bpt:	base pointer table to free
*/

void freeNodesMem(basePtrTab * bpt){
	int i;
	for(i=0;i<=bpt->lastP;i++){
		if(bpt->table[i] != NULL) free(bpt->table[i]);
	}
}

/*
	One-time malloc that allocates the memory needed for a sequence of length n
	@bytes:	Total bytes to be allocated
	
	Returns a char pointer to the first position of the allocated block of memory
*/

char * oneTimeMalloc(uint64_t bytes){
	//uint64_t tnode_n = bytes/ksize;
	//uint64_t tnode_s = (bytes-ksize+1) - tnode_n;
	//printf("Allocating %"PRIu64" bytes (%"PRIu64" MB) for %"PRIu64" nodes_n and %"PRIu64" nodes_s\nSequence length %"PRIu64"\nK-size = %"PRIu16"\n", totalAlloc, totalAlloc/(1024*1024), tnode_n, tnode_s, bytes, ksize);
	char * memPointer = (char *) malloc(bytes*sizeof(char));
	if(memPointer == NULL) terror("Could not allocate memory for binary tree");
	return memPointer;
}

/*
	This functions returns the next free position in the block of memory
	@typeOfNode:	Whether its a Node_N (0) or a Node_S (1)
	@currMem:		The current position of the memory pointer
	
	Returns a void pointer to the new position
*/


char * askForMem(int typeOfNode, char ** currMem){
	
	if(typeOfNode == 0){
		*currMem += sizeof(Node_N);		
		return (*currMem - sizeof(Node_N));
	} 
	if(typeOfNode == 1){
		*currMem += sizeof(Node_S);	
		return (*currMem - sizeof(Node_S));
	}
	terror("Bad type of node: memory get for new tree node \n");
}

/*
	Allocates memory to hold all kmer positions needed
	@totalMers:	total number of kmers
	
	Returns a pointer to the allocated region
*/

char * allocMemoryForPositions(uint32_t totalMers){
	char * mem = (char *) malloc(totalMers * sizeof(l_item));
	if(mem == NULL) terror("Could not allocate memory for kmers positions");
	return mem;
}

/*
	Gets the next free position for an l_item by returning the offset that has to be added to the base memory pointer
*/

uint32_t getPositionOffset(){
	static uint32_t consumed = 0;
	consumed += (uint32_t) sizeof(l_item); //Each position consumes the size of an l_item
	if(consumed >= UINT32_MAX) terror("Reached max number of positions to store");
	return (consumed - (uint32_t)sizeof(l_item));
}

/*
	For a newly created tree node, sets the node offset to the current one, calculates what absolute pointer position
	should the kmer position be stored in and stores it
	@node:	A void pointer to a node_N or node_S
	@pos:	The kmer position
	@baseMem:	An absolute pointer to the starting position of the allocated heap for positions
*/

void setFirstPosition(Node_N * node, uint32_t pos, char * baseMem){
	uint32_t posOff = getPositionOffset();
	node->n_ltable = posOff;
	/*
	if(getTypeOfNode(node) == 0){
		((Node_N *) node)->n_ltable = posOff;
	}else{
		((Node_S *) node)->n_ltable = posOff;
	}
	*/
	l_item * aux = (l_item *) (baseMem+(uint64_t)posOff);
	aux->pos = pos;
	aux->offsetNext = 0; //So we know there is not a next one
	ramUsage(2);
}

/*
	Returns the first position where a kmer appears
	@offsetPos:	The offset base position from the node struct
	@baseMem:	The pointer to the allocated memory
	
	Returns the position in the fasta file
*/

uint32_t getPosOfNode(uint32_t offsetPos, char * baseMem){
	l_item * aux = (l_item *) (baseMem + (uint64_t)offsetPos);
	return aux->pos;
}

/*
	Links a new position to a offset-based linked list of positions
	@node:	The node for which to add a new position
	@baseMem:	The offset pointer to the memory heap of positions
	@pos:	The position in the fasta sequence to add
*/

void linkNewPos(Node_N * node, char * baseMem, uint32_t pos){
	l_item * first;
	//l_item * aux;
	l_item * next;
	uint32_t offsetFirst;
	
	//Convert to l_item the first position
	first = (l_item *) (baseMem + ((uint64_t)(node)->n_ltable));
	//aux = first;
	offsetFirst = first->offsetNext;
	/*
	if(typeOfNode == 0){
		aux = (l_item *) (baseMem + ((uint64_t)((Node_N *) node)->n_ltable));
	}else{
		aux = (l_item *) (baseMem + ((uint64_t)((Node_S *) node)->n_ltable));
	}
	*/
	
	/*
	if(aux->offsetNext != 0){ //Get second position
		
		aux = (l_item *) (baseMem + (uint64_t)aux->offsetNext);
		
	}
	*/
	//aux holds the second position, insert now between first and second to make complexity O(2)
	
	
	first->offsetNext = getPositionOffset(); //next free position
	
	next = (l_item *) (baseMem + (uint64_t)first->offsetNext);
	next->pos = pos;
	next->offsetNext = offsetFirst;
	
	//Add bytes used
	ramUsage(2);
	
}

/*
	Traverses a list of offset-based linked lists
	@node	The node for which to traverse the positions
	@baseMem	The offset for the allocated heap of positions
	@f	File handler to write to
*/

void traversePosLists(Node_N * node, char * baseMem, FILE * f){
	l_item * aux;
	//int typeOfNode = getTypeOfNode(node);
	
	//Convert to l_item the first position
	
	aux = (l_item *) (baseMem + node->n_ltable);
	//printf("POS: %"PRIu32", ", aux->pos);
	fprintf(f, "\tPOS: %"PRIu32, getPosOfNode(node->n_ltable, baseMem));
	/*
	if(typeOfNode == 0){
		aux = (l_item *) (baseMem + ((Node_N *) node)->n_ltable);
		//printf("POS: %"PRIu32", ", aux->pos);
		fprintf(f, "\tPOS: %"PRIu32, getPosOfNode(((Node_N *) node)->n_ltable, baseMem));
		//printf("At %p\n", aux);
		
	}else{
		aux = (l_item *) (baseMem + ((Node_S *) node)->n_ltable);
		//printf("POS: %"PRIu32", ", aux->pos);
		fprintf(f, "\tPOS: %"PRIu32, getPosOfNode(((Node_S *) node)->n_ltable, baseMem));
		//printf("At %p\n", aux);
		
	}
	*/
	
	while(aux->offsetNext != 0){ //Until we find the last positions node
		//currPosId = aux->offsetNext;
		aux = (l_item *) (baseMem + (uint64_t)aux->offsetNext);
		fprintf(f, ", %"PRIu32, aux->pos);
		
		//printf("At %p\n", aux);
	}
	fprintf(f, "\n");
}

/*
	Traverses the tree in preorder and shows the list of positions for each node
	@n:	The node from which to traverse
	@baseMem:	The offset pointer to the positions block of memory
*/

void traverseTreeAndPositions(Node_N * n, char * baseMem, basePtrTab * bpt){
	traversePosLists(n, baseMem, stdout);
	
	showNode_N(n);
	if(n->left != 0) traverseTreeAndPositions(getPointerFromOffset(bpt, n->left, n->llevel), baseMem, bpt);	
	if(n->right != 0) traverseTreeAndPositions(getPointerFromOffset(bpt, n->right, n->rlevel), baseMem, bpt);
	/*
	if(getTypeOfNode(n) == 0){ //Type Non-overlapping
		castNCurr = (Node_N *) n;
		showNode_N(castNCurr);
		if(castNCurr->left != NULL) traverseTreeAndPositions((void *) castNCurr->left, baseMem);	
		if(castNCurr->right != NULL) traverseTreeAndPositions((void *) castNCurr->right, baseMem);
	}else{ //Type overlapping
		castSCurr = (Node_S *) n;
		showNode_S(castSCurr);
		if(castSCurr->left != NULL) traverseTreeAndPositions((void *) castSCurr->left, baseMem);	
		if(castSCurr->right != NULL) traverseTreeAndPositions((void *) castSCurr->right, baseMem);	
	}
	*/
}


/*
	Writes the tree to disk in a similar manner as GECKO
	@n:	The node from which to traverse
	@baseMem:	The offset pointer to the positions block of memory
	@f:	File handler (use stdout for standard printing) to write the dictionary to
	@bpt:	Table of base pointers
*/

void writeDictionary(Node_N * n, char * baseMem, FILE * f, basePtrTab * bpt){

	unsigned char word[8];
	char kmer[32];	
	memcpy(word, n->b, 8);
	showWord(word, kmer, KSIZE);
	
	if(n->left != 0){
		writeDictionary(getPointerFromOffset(bpt, n->left, n->llevel), baseMem, f, bpt);
	} 
	
	fprintf(f, "%s:*", kmer);
	traversePosLists(n, baseMem, f);
	
	
	
    if(n->right != 0){
    	writeDictionary(getPointerFromOffset(bpt, n->right, n->rlevel), baseMem, f, bpt);
    	//fprintf(f, "%s:*", kmer);
		//traversePosLists(n, baseMem, f);
	} 
	

	/*
	if(getTypeOfNode(n) == 0){ //Type Non-overlapping
		castNCurr = (Node_N *) n;
		//showNode_N(castNCurr);
		showWord(castNCurr->b, kmer, BYTES_IN_WORD*4);
		
		int i;
		for(i=0;i<8;i++)LAST_N[i] = castNCurr->b[i];
		
		fprintf(f, "%s:*", kmer);
		traversePosLists(n, baseMem, f);
		if(castNCurr->left != NULL) writeDictionary((void *) castNCurr->left, baseMem, f);	
		if(castNCurr->right != NULL) writeDictionary((void *) castNCurr->right, baseMem, f);
	}else{ //Type overlapping
	
		castSCurr = (Node_S *) n;
		shift_word_left(LAST_N);
		addNucleotideToWord(LAST_N, 'f', bitsToChar(castSCurr->b));
		showWord(LAST_N, kmer, BYTES_IN_WORD*4);
		fprintf(f, "%s:", kmer);
		traversePosLists(n, baseMem, f);
		if(castSCurr->left != NULL) writeDictionary((void *) castSCurr->left, baseMem, f);	
		if(castSCurr->right != NULL) writeDictionary((void *) castSCurr->right, baseMem, f);	
	}
	*/
}
