#ifndef DS_
#define DS_


#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Chess.h"

#define OUT_OF_BOARD -1


/* The basic data structers we use*/
struct listNode{
	void *data;
	struct listNode *next;
	size_t size;
};

typedef struct listNode listNode;

typedef struct {
	listNode *first;
}linkedList;

typedef struct {
	char x;
	int y;
}pos;

typedef struct {
	pos origin;
	pos dest;
	char promType;
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
move* newMove(pos, pos,char);
linkedList* newLinkedList();
int compPos(pos, pos); // returns 1 if same position, o.w. 0
int isLegalPos(pos); // returns 1 if the position is legal, o.w. 0
//piece* newPiece(char, int, pos);
void printMove(move *);
void printMoves(linkedList*);
void initMailBox120();
void set(char*, pos, char, int);
//void initPiece(piece*, char , int, pos);

extern int mailbox[];

#endif 


