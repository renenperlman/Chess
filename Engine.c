#include "Engine.h"



/* Return the color of the piece in the given index on the give board
assumes that there is a piece in the location
1 - white, 0 - black , -1 - empty*/
int colorOfLoc(char* board, int ind){
	if (board[ind] == WHITE_P || board[ind] == WHITE_B ||
		board[ind] == WHITE_K || board[ind] == WHITE_N ||
		board[ind] == WHITE_Q || board[ind] == WHITE_R) return WHITE;
	if (board[ind] == OUT_OF_BOARD || board[ind] == EMPTY)
	{
		return OUT_OF_BOARD;
	}
	return BLACK;
}

/*adds to the list nodes containing all the possible moves by the given piece*/
void genMoves(piece *p, linkedList *moves, char board[]){
	if (p->type!=WHITE_P && p->type!=BLACK_P)
	{
		for (int i = 0; i < p->numOfDirections; i++)
		{
			int ind = posToInd(p->position);
			while (1)
			{
				ind += p->directions[i];
				if (board[ind] == OUT_OF_BOARD) break;
				if (colorOfLoc(board, ind) == p->color) break;
				int capture = board[ind] == EMPTY ? 0 : 1;
				insertNode(moves,newNode(newMove(p, indToPos(ind), capture), sizeof(move))); // a possible move - add it to the list
				if (capture) break;
				if (p->singleMove) break; // continue to new direction
			}
		}
	}
	else 
	{
		int ind = posToInd(p->position);
		if (board[ind + p->directions[0]]==EMPTY) // make a step forward
			insertNode(moves,newNode(newMove(p, indToPos(ind + p->directions[0]),0), sizeof(move)));
		for (int i = 1; i < 3; i++)
		{
			if (colorOfLoc(board, ind + p->directions[i]) == 1-p->color) // capture
				insertNode(moves,newNode(newMove(p, indToPos(ind + p->directions[i]),1), sizeof(move)));
		}
	}
}

/*returns a list containing all the possible moves by the given player*/
linkedList* getMoves(piece *pieces, char* board, int player, int check){
	linkedList* moves = newLinkedList();
	int shift = player == WHITE ? 0 : 16;
	for (int i = 0; i < NUMOFPIECES/2; i++){
		if (pieces[i + shift].captured || pieces[i + shift].color != player)
		{
			continue;
		}
		genMoves(pieces + i + shift, moves, board);
	}
	if (check)
	{
		updateMoveList(moves, pieces, player, board);
	}
	return moves;
}
/* assumes that the given player is in check,
remove all moves that keeps in him check*/
void makeMove(move* m, piece* pieces, char* board){
	pos position = m->p->position;
	int ind = posToInd(position);
	board[ind] = EMPTY; // update board
	pos dest = m->dest;
	int destInd = posToInd(dest);
	board[destInd] = m->p->type; // update board
	if (m->capture){
		for (int i = 0; i < NUMOFPIECES; i++){ // update pieces array
			if (compPos(pieces[i].position, dest))
			{
				pieces[i].captured = 1; // update captured piece
				break;
			}
		}
	}
	for (int i = 0; i < NUMOFPIECES; i++){ 
		if (compPos(pieces[i].position, m->p->position) && !pieces[i].captured)
		{
			pieces[i].position = dest; // update moving piece postion
			break;
		}
	}
}

void updateMoveList(linkedList* moves, piece* pieces, int player, char* board){
	listNode *prev = NULL;
	listNode *curr = moves->first;
	while (curr != NULL)
	{
		char* newBoard = (char*)malloc(120 * sizeof(char));
		memcpy(newBoard, board, 120 * sizeof(char));
		piece* newPieces = (piece*)malloc(NUMOFPIECES * sizeof(piece));
		memcpy(newPieces, pieces, NUMOFPIECES * sizeof(piece));
		makeMove((move*)curr->data, newPieces, newBoard);
		if (isCheck(newPieces,newBoard,player))
		{
			curr = removeNode(moves, prev, curr,1);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
		free(newBoard);
		free(newPieces);
	}
}

int score(piece *pieces,char* board, int player){
	linkedList *moves = getMoves(pieces, board, 1 - player, isCheck(pieces, board, 1 - player));
	if (moves->first == NULL){ // no possible moves for other player
		if (isCheck(pieces, board, 1 - player)){ // check
			freeList(moves,1);
			return 500; // the player wins
		}
		else // not in check
		{
			freeList(moves, 1);
			return -0; // tie
		}
	}
	if (moves->first == NULL){ // no possible moves
		if (isCheck(pieces, board, player)){ // check
			freeList(moves, 1);
			return -500; // the player lost
		}
		else // not in check
		{
			freeList(moves, 1);
			return -0; // tie
		}
	}
	int score = 0;
	for (int i = 0; i < NUMOFPIECES; i++)
	{
		if (pieces[i].captured)
		{
			continue;
		}
		char type = pieces[i].type;
		if (type == WHITE_P || type == BLACK_P)
			score += 1 * (2 * pieces[i].color - 1);
		else if (type == WHITE_N || type == BLACK_N || type == WHITE_B || type == BLACK_B)
			score += 3 * (2 * pieces[i].color - 1);
		else if (type == WHITE_R || type == BLACK_R)
			score += 5 * (2 * pieces[i].color - 1);
		else if (type == WHITE_Q || type == BLACK_Q)
			score += 9 * (2 * pieces[i].color - 1);
		else 
			score += 400 * (2 * pieces[i].color - 1);
	}
	freeList(moves, 1);
	return player==WHITE ? score : -1 * score;
}

/*promote the given pawn to the given type*/
void promote(piece* p, char type){
	p->type = type;
	if (type == WHITE_B || type == BLACK_B)
	{
		int directions[] = B_DIRECTIONS;
		p->directions = directions;
		p->numOfDirections = 4;
		p->singleMove = 0;
	}
	else if (type == WHITE_N || type == BLACK_N)
	{
		int directions[] = N_DIRECTIONS;
		p->directions = directions;
		p->numOfDirections = 8;
	}
	else if (type == WHITE_Q || type == BLACK_Q)
	{
		int directions[] = QnK_DIRECTIONS;
		p->directions = directions;
		p->numOfDirections = 8;
		p->singleMove = 0;
	}
	else{
		int directions[] = R_DIRECTIONS;
		p->directions = directions;
		p->numOfDirections = 4;
		p->singleMove = 0;
	}
}

/*returns 1 if the given player threaten the given position
otherwise, return 0*/
int isThreat(pos p, piece* pieces, char* board, int player){
	linkedList* moves = newLinkedList();
	int shift = player == WHITE ? 0 : 16;
	for (int i = 0; i < NUMOFPIECES/2; i++){
		if (pieces[i + shift].captured || pieces[i + shift].color != player)
		{
			continue;
		}
		genMoves(pieces + i + shift, moves, board);
	}
	listNode* node = moves->first;
	while (node!=NULL)
	{
		if (compPos(((move*)node->data)->dest, p) == 1){
			freeList(moves, 1);
			return 1;
		}
		node = node->next;
	}
	freeList(moves, 1);
	return 0;
}

/*returns 1 if the given player is in check in the given board*/
int isCheck(piece* pieces, char* board, int player){
	char type = player ? WHITE_K : BLACK_K;
	int shift = player == WHITE ? 0 : 16;
	pos kingPos;
	for (int i = 0; i < NUMOFPIECES/2; i++)
	{
		if (pieces[i + shift].type == type){
			kingPos = pieces[i + shift].position;
			break;
		}
	}
	return isThreat(kingPos, pieces, board, 1 - player);
}

/*returns 1 if the given player has a possible move*/
int hasMoves(piece* pieces, char* board, int player){
	return (getMoves(pieces, board, player, isCheck(pieces, board, player))->first != NULL);
}