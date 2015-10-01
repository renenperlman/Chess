#include "DS.h"

/*a premade table showing the mailbox index
of each tile of the board*/
int mailbox[64] = { 
	91, 92, 93, 94, 95, 96, 97, 98,
	81, 82, 83, 84, 85, 86, 87, 88,
	71, 72, 73, 74, 75, 76, 77, 78,
	61, 62, 63, 64, 65, 66, 67, 68,
	51, 52, 53, 54, 55, 56, 57, 58,
	41, 42, 43, 44, 45, 46, 47, 48,
	31, 32, 33, 34, 35, 36, 37, 38,
	21, 22, 23, 24, 25, 26, 27, 28 
};

/*a table showing the position on board 
of each mailbox index*/
char mailbox120[120][2];

void initMailBox120(){
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 12; j++){
			if (i < 2 || i > 9 || j == 0 || j == 11) // out of board is marked by <'x',0>
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


/*frees the given node and its data*/
void freeNode(listNode* node){
	free(node->data);
	free(node);
}

/*frees the given linked list
frees all the nodes and their data*/
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

/*allocate memory for the given size
and copy the data into it*/
void* cloneData(void* data, size_t n){
	char* cloneData = (char*)malloc(n);
	if (cloneData == NULL)
	{
		print_message("Error: standard function malloc has failed");
		print_message("Terminating program");
		exit(1);
	}
	memcpy(cloneData, data, n);
	return cloneData;
}

/*clone the given node
and also its data*/
listNode* cloneNode(listNode* node){
	listNode* clone = (listNode*)malloc(sizeof(listNode));
	if (clone == NULL)
	{
		print_message("Error: standard function malloc has failed");
		print_message("Terminating program");
		exit(1);
	}
	char* data = cloneData(node->data, node->size);
	clone->data = data;
	clone->next = NULL;
	return clone;
}

/*creates a new node and allocated 
memory for data in the given size*/
listNode* newNode(void* data, size_t n){
	listNode* node = (listNode*)malloc(sizeof(listNode));
	if (node == NULL)
	{
		print_message("Error: standard function malloc has failed");
		print_message("Terminating program");
		exit(1);
	}
	node->data = data;
	node->size = n;
	node->next = NULL;
	return node;
}

/*insert the given node to the start of the list*/
void insertNode(linkedList* list, listNode* node){
	node->next = list->first;
	list->first = node;
}

/* removes the given node from the given list.
the other node is the prev node
returns the updated curr node*/
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

/*returns the mailbox index of the given position*/
int posToInd(pos p){
	return mailbox[p.x - 'a' + 8 * (p.y-1)];
}

/*returns the board position of the given mailbox index*/
pos indToPos(int ind){
	pos p = { mailbox120[ind][0], (int)mailbox120[ind][1] };
	return p;
}

move* newMove(pos origin, pos dest, char promType){
	move* m = (move*)malloc(sizeof(move));
	if (m == NULL)
	{
		print_message("Error: standard function malloc has failed");
		print_message("Terminating program");
		exit(1);
	}
	m->dest = dest;
	m->origin = origin;
	m->promType = promType;
	return m;
}

/*creates a new empty linked list*/
linkedList* newLinkedList(){
	linkedList* list = (linkedList*)malloc(sizeof(linkedList));
	if (list == NULL)
	{
		print_message("Error: standard function malloc has failed");
		print_message("Terminating program");
		exit(1);
	}
	list->first = NULL;
	return list;
}

/*returns 1 if the positions are equal
returns 0 otherwise*/
int compPos(pos p1, pos p2){
	return (p1.x == p2.x) && (p1.y == p2.y);
}

/*returns 1 if the given position is within the board*/
int isLegalPos(pos p){
	return (p.x >= 'a' && p.x<='h' && p.y>=1 && p.y<=8);
}

/*prints the given move*/
void printMove(move *m){
	printf("<%c,%d> to <%c,%d>", m->origin.x, m->origin.y, m->dest.x, m->dest.y);
	if (m->promType == 'q')
	{
		printf(" queen");
	}
	else if (m->promType == 'b')
	{
		printf(" bishop");
	}
	else if (m->promType == 'r')
	{
		printf(" rook");
	}
	else if (m->promType == 'n')
	{
		printf(" knight");
	}
	printf("\n");
}

/*prints all the moves in the list*/
void printMoves(linkedList *moves){
	listNode *node = moves->first;
	while (node!=NULL)
	{
		printMove(node->data);
		node = node->next;
	}
}

/*sets the piece of the color in the position on the board
checks if the position is legal
checks if there aren't too many pieces of the given type*/
void set(char* board, pos p, char type, int color){
	if (!isLegalPos(p))	// illegal pos
	{
		print_message(WRONG_POSITION);
		return;
	}
	int count = 0;
	pos searcher;
	for (int x = 'a'; x < 'i'; x++) // counts how many pieces of the given type are on the board
	{
		searcher.x = x;
		for (int y = 1; y < 9; y++)
		{
			searcher.y = y;
			if (board[posToInd(searcher)] == type)
			{
				count++;
			}
		}
	}
	int maxCount; // max amount of pieces of the given type
	if (type == WHITE_P || type == BLACK_P)
	{
		maxCount = 8;
	}
	else if (type == WHITE_K || type == BLACK_K || type == WHITE_Q || type == BLACK_Q)
	{
		maxCount = 1;
	}
	else
	{
		maxCount = 2;
	}
	if (count >= maxCount){	// too many pieces of the same type
		print_message(NO_PIECE); // illegal board
		return;
	}
	board[posToInd(p)] = type; // set the piece
}