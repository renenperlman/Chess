#include "DS.h"

int mailbox[64] = { 91, 92, 93, 94, 95, 96, 97, 98,
81, 82, 83, 84, 85, 86, 87, 88,
71, 72, 73, 74, 75, 76, 77, 78,
61, 62, 63, 64, 65, 66, 67, 68,
51, 52, 53, 54, 55, 56, 57, 58,
41, 42, 43, 44, 45, 46, 47, 48,
31, 32, 33, 34, 35, 36, 37, 38,
21, 22, 23, 24, 25, 26, 27, 28 };

char mailbox120[120][2];

void initMailBox120(){
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 12; j++){
			if (i < 2 || i > 9 || j == 0 || j == 11)
			{
				mailbox120[10 * i + j][0] = 'x';
				mailbox120[10 * i + j][1] = 0;
			}
			else{
				mailbox120[10 * i + j][0] = j-1+'a';
				mailbox120[10 * i + j][1] = 10-i;
			}
		}
	}
}



void freeNode(listNode* node, char flag){
	if (flag)
	{
		free(node->data);
	}
	free(node);
}
void freeList(linkedList* list, char flag){
	listNode* curr = list->first;
	while (curr!=NULL)
	{
		listNode* next = curr->next;
		freeNode(curr, flag);
		curr = next;
	}
	free(list);
}

void* cloneData(void* data, size_t n){
	char* cloneData = (char*)malloc(n);
	memcpy(cloneData, data, n);
	return cloneData;
}

listNode* cloneNode(listNode* node){
	listNode* clone = (listNode*)malloc(sizeof(listNode));
	char* data = cloneData(node->data, node->size);
	clone->data = data;
	clone->next = NULL;
	return clone;
}



listNode* newNode(void* data, size_t n){
	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->data = data;
	node->size = n;
	node->next = NULL;
	return node;
}

void insertNode(linkedList* list, listNode* node){
	node->next = list->first;
	list->first = node;
}

/* removes the given node from the given list.
the other node is the prev node
returns the updated curr node*/
listNode* removeNode(linkedList* list , listNode* prev , listNode* curr, char flag){
	if (prev==NULL)
	{
		list->first = curr->next;
		freeNode(curr, flag);
		return list->first;
	}
	prev->next = curr->next;
	freeNode(curr, flag);
	return prev->next;
}

int posToInd(pos p){
	return mailbox[p.x - 'a' + 8 * (p.y-1)];
}

pos indToPos(int ind){
	pos p = { mailbox120[ind][0], (int)mailbox120[ind][1] };
	return p;
}

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
	return list;
}

int compPos(pos p1, pos p2){
	return (p1.x == p2.x) && (p1.y == p2.y);
}

int isLegalPos(pos p){
	return (p.x >= 'a' && p.x<='h' && p.y>=1 && p.y<=8);
}

piece* newPiece(char type, int color, pos position){
	type = color == WHITE ? (char)tolower(type) : (char)toupper(type);
	piece *newP = (piece*)malloc(sizeof(piece));
	newP->captured = 0;
	newP->color = color;
	newP->position = position;
	newP->type = type;
	if (type == 'm'){
		int directions[] = WHITE_P_DIRECTIONS;
		newP->numOfDirections = 3;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 1;
	}
	else if (type == 'M'){
		int directions[] = BLACK_P_DIRECTIONS;
		newP->numOfDirections = 3;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 1;
	}
	else if (type == WHITE_B || type == BLACK_B)
	{
		int directions[] = B_DIRECTIONS;
		newP->numOfDirections = 4;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 0;
	}
	else if (type == WHITE_N || type == BLACK_N)
	{
		int directions[] = N_DIRECTIONS;
		newP->numOfDirections = 8;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 1;
	}
	else if (type == WHITE_Q || type == BLACK_Q)
	{
		int directions[] = QnK_DIRECTIONS;
		newP->numOfDirections = 8;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 0;
	}
	else if (type == WHITE_K || type == BLACK_K)
	{
		int directions[] = QnK_DIRECTIONS;
		newP->numOfDirections = 8;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 1;
	}
	else{
		int directions[] = R_DIRECTIONS;
		newP->numOfDirections = 4;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 0;
	}
	return newP;
}

void initPiece(piece *newP, char type, int color, pos position){
	type = color == WHITE ? (char)tolower(type) : (char)toupper(type);
	newP->captured = 0;
	newP->color = color;
	newP->position = position;
	newP->type = type;
	if (type == 'm'){
		int directions[] = WHITE_P_DIRECTIONS;
		newP->numOfDirections = 3;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 1;
	}
	else if (type == 'M'){
		int directions[] = BLACK_P_DIRECTIONS;
		newP->numOfDirections = 3;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 1;
	}
	else if (type == WHITE_B || type == BLACK_B)
	{
		int directions[] = B_DIRECTIONS;
		newP->numOfDirections = 4;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 0;
	}
	else if (type == WHITE_N || type == BLACK_N)
	{
		int directions[] = N_DIRECTIONS;
		newP->numOfDirections = 8;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 1;
	}
	else if (type == WHITE_Q || type == BLACK_Q)
	{
		int directions[] = QnK_DIRECTIONS;
		newP->numOfDirections = 8;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 0;
	}
	else if (type == WHITE_K || type == BLACK_K)
	{
		int directions[] = QnK_DIRECTIONS;
		newP->numOfDirections = 8;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 1;
	}
	else{
		int directions[] = R_DIRECTIONS;
		newP->numOfDirections = 4;
		newP->directions = cloneData(directions, sizeof(int)*newP->numOfDirections);
		newP->singleMove = 0;
	}
	return ;
}

void printMove(move *m){
	printf("<%c,%d> to <%c,%d>\n", m->p->position.x, m->p->position.y,m->dest.x,m->dest.y);
}

void printMoves(linkedList *moves){
	listNode *node = moves->first;
	while (node!=NULL)
	{
		printMove(node->data);
		node = node->next;
	}
}

void set(char* board, piece* pieces, pos p, char type, int color){
	if (!isLegalPos(p))	// illegal pos
	{
		print_message(WRONG_POSITION);
	}
	for (int i = 0; i < NUMOFPIECES; i++)
	{
		if (compPos(pieces[i].position, p) && pieces[i].captured == 0)
		{
			pieces[i].captured = 1;	// remove the piece from the pos
			break;
		}
	}
	int flag = 0;
	int shift = color == WHITE ? 0 : 16;
	for (int i = 0; i < NUMOFPIECES/2; i++) // search for a free piece of the given type
	{
		if (pieces[i + shift].color == color && pieces[i + shift].type == type && pieces[i + shift].captured == 1)
		{
			pieces[i + shift].position = p;
			board[posToInd(p)] = type;
			pieces[i + shift].captured = 0;
			pieces[i + shift].moved = 1;
			flag = 1;
			break;
		}
	}
	if (!flag){	// couldnt find a free piece of the given type
		print_message(NO_PIECE); // illegal board
	}
}