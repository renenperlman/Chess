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
linkedList* getMoves(piece *pieces, char* board, int player){
	linkedList* moves = newLinkedList();
	for (int i = 0; i < NUMOFPIECES; i++){
		if (pieces[i].captured || pieces[i].color!=player)
		{
			continue;
		}
		genMoves(pieces + i, moves, board);
	}
	if (isCheck(pieces,board,player))
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
	m->p->position = dest; // update moving piece postion
}

void updateMoveList(linkedList* moves, piece* pieces, int player, char* board){
	listNode *prev = NULL;
	listNode *curr = moves->first;
	while (curr->next!=NULL)
	{
		char* newBoard = (char*)malloc(120 * sizeof(char));
		memcpy(newBoard, board, 120 * sizeof(char));
		piece* newPieces = (piece*)malloc(NUMOFPIECES * sizeof(piece));
		memcpy(newPieces, pieces, NUMOFPIECES * sizeof(piece));
		makeMove((move*)curr->data, newPieces, newBoard);
		if (isCheck(newPieces,newBoard,player))
		{
			curr = removeNode(moves, prev, curr);
		}
		prev = curr;
		curr = curr->next;
		free(newBoard);
		free(newPieces);
	}
}

int score(piece *pieces,char* board, int player){
	if (getMoves(pieces, board, 1 - player)->first == NULL){ // no possible moves for other player
		if (isCheck(pieces, board, 1 - player)){ // check
			return 500; // the player wins
		}
		else // not in check
		{
			return -400; // tie
		}
	}
	if (getMoves(pieces, board, player)->first == NULL){ // no possible moves
		if (isCheck(pieces, board, player)){ // check
			return -500; // the player lost
		}
		else // not in check
		{
			return -400; // tie
		}
	}
	int score = 0;
	for (int i = 0; i < NUMOFPIECES; i++)
	{
		if (pieces[i].captured)
		{
			continue;
		}
		if (pieces[i].type == 'p')
			score += 1 * (2 * pieces[i].color-1);
		else if (pieces[i].type == 'k' || pieces[i].type == 'b')
			score += 3 * (2 * pieces[i].color - 1);
		else if (pieces[i].type == 'r')
			score += 5 * (2 * pieces[i].color - 1);
		else if (pieces[i].type == 'q')
			score += 9 * (2 * pieces[i].color - 1);
		else 
			score += 400 * (2 * pieces[i].color - 1);
	}
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
	for (int i = 0; i < NUMOFPIECES; i++){
		if (pieces[i].captured || pieces[i].color != player)
		{
			continue;
		}
		genMoves(pieces + i, moves, board);
	}
	listNode* node = moves->first;
	while (node!=NULL)
	{
		if (compPos(((move*)node->data)->dest, p) == 1)
			return 1;
		node = node->next;
	}
	return 0;
}

/*returns 1 if the given player is in check in the given board*/
int isCheck(piece* pieces, char* board, int player){
	char type = player ? WHITE_K : BLACK_K;
	pos kingPos;
	for (int i = 0; i < NUMOFPIECES; i++)
	{
		if (pieces[i].type == type){
			kingPos = pieces[i].position;
			break;
		}
	}
	return isThreat(kingPos, pieces, board, 1 - player);
}

/*returns 1 if the given player has a possible move*/
int hasMoves(piece* pieces, char* board, int player){
	return (getMoves(pieces, board, player)->first != NULL);
}