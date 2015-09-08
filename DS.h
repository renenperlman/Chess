#include <stdio.h>
#include <stdlib.h>
#include "Chess.h"

#define OUT_OF_BOARD -1

/* The basic data structers we use*/
typedef struct {
	void *data;
	struct listNode *next;
	size_t size;
}listNode;

typedef struct {
	listNode *first;
}linkedList;

typedef struct {
	char x;
	int y;
}pos;

typedef struct {
	char type;
	int color; // 1 - white, 0 - black
	char moved; // 0 - hasn't moved yet, 1 - moved
	pos position;
	char singleMove; // 0 - piece can move multiple times in the same direction, 1 - can't
	int numOfDirections; // the number of possible directions the piece can move
	int *directions; // an arry with the directions the piece can move in (using 10x12 mailbox)
	int captured; // 0 - piece is still in game, 1 - piece is dead
}piece;

typedef struct {
	piece *p;
	pos dest;
	int capture; // 0 - not a capture move, 1 - capture move
}move;

/* Prototype for basic functions */
void freeNode(listNode*); // frees the given node and also its data
void freeList(linkedList*); // frees the given linked list
void* cloneData(void*, size_t); // copy the content of the given pointer
listNode* cloneNode(listNode*); // copies the given node and its data
linkedList* cloneLinkedList(linkedList*); // copies the given linked list
listNode* newNode(void*, size_t); // guess what this one does??
void insertNode(linkedList*, listNode*); // adds the node to the begining of the list
listNode* removeNode(linkedList*, listNode*, listNode*); // removes the given node from the given list.
															//the other node is the prev node
															// returns the updated curr node
int posToInd(pos); // returns the mailbox number of the given location
pos indToPos(int); // returns the position of the given mailbox number
move* newMove(piece*, pos,int);
linkedList* newLinkedList();
int compPos(pos, pos); // returns 1 if same position, o.w. 0

