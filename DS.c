#include "DS.h"

void freeNode(listNode* node){
	free(node->data);
	free(node);
}
void freeList(linkedList* list){
	listNode* curr = list->first;
	while (curr!=NULL)
	{
		listNode* next = curr->next;
		freeNode(curr);
		curr = next;
	}
	free(list);
}

void* cloneData(void*, size_t); // copy the content of the given pointer

listNode* cloneNode(listNode* node){
	listNode* clone = (listNode*)malloc(sizeof(listNode));
}

linkedList* cloneLinkedList(linkedList*); // copies the given linked list

listNode* newNode(void* data, size_t n){
	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->data = data;
	node->size = n;
	node->next = NULL;
}

void insertNode(linkedList* list, listNode* node){
	node->next = list->first;
	list->first = node;
}

listNode* removeNode(linkedList* list , listNode* prev , listNode* curr){
	if (prev==NULL)
	{
		list->first = curr->next;
		freeNode(curr);
		return list->first;
	}
	prev->next = curr->next;
	freeNode(curr);
	return prev->next;
}

int posToInd(pos p){
	return 21 + 8 * (p.x - 'a') + p.y;
}

pos indToPos(int ind); // returns the position of the given mailbox number

move* newMove(piece* p, pos dest, int capture){
	move* m = (move*)malloc(sizeof(move));
	m->capture = capture;
	m->dest = dest;
	m->p = p;
	return m;
}

linkedList* newLinkedList(){
	linkedList* list = (linkedList*)malloc(sizeof(linkedList));
	list->first = NULL;
}

int compPos(pos p1, pos p2){
	return (p1.x == p2.x) && (p1.y == p2.y);
}