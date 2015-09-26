#include "Engine.h"

int WHITE_P_DIRECTIONS[3] = { -10, -9, -11 };
int BLACK_P_DIRECTIONS[3] = { 10, 9, 11 };
int N_DIRECTIONS[8] = { -21, -19, -12, -8, 8, 12, 19, 21 };
int B_DIRECTIONS[4] = { -11, -9, 9, 11 };
int R_DIRECTIONS[4] = { -10, -1, 1, 10 };
int QnK_DIRECTIONS[8] = { -11, -10, -9, -1, 1, 9, 10, 11 };

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
void genMoves(pos p, linkedList *moves, char board[]){
	char type = board[posToInd(p)];
	char* newBoard = (char*)malloc(120 * sizeof(char));
	int color = colorOfLoc(board, posToInd(p));
	if (type != WHITE_P && type != BLACK_P) // not a pawn
	{
		int numOfDirections = (type == WHITE_R || type == BLACK_R 
			|| type == WHITE_B || type == BLACK_B) ? 4 : 8; // the number of possible move directions
		int* directions;
		char singleMove; //  a flag if the piece can move only once in a direction
		if (type == WHITE_R || type == BLACK_R)
		{
			directions = R_DIRECTIONS;
			singleMove = 0;
		}
		else if (type == WHITE_N || type == BLACK_N)
		{
			directions = N_DIRECTIONS;
			singleMove = 1;
		}
		else if (type == WHITE_B || type == BLACK_B)
		{
			directions = B_DIRECTIONS;
			singleMove = 0;
		}
		else if (type == WHITE_Q || type == BLACK_Q)
		{
			directions = QnK_DIRECTIONS;
			singleMove = 0;
		}
		else
		{
			directions = QnK_DIRECTIONS;
			singleMove = 1;
		}
		for (int i = 0; i < numOfDirections; i++)
		{
			int ind = posToInd(p);
			while (1)
			{
				ind += directions[i];
				if (board[ind] == OUT_OF_BOARD) break; // out of bound
				if (colorOfLoc(board, ind) == color) break; // same piece color
				int capture = board[ind] == EMPTY ? 0 : 1;
				move *m = newMove(p, indToPos(ind), capture);
				memcpy(newBoard, board, 120 * sizeof(char));
				makeMove(m, newBoard);
				if (isCheck(newBoard,color))
				{
					free(m);
					if (singleMove || capture)
					{
						break;
					}
					continue;
				}
				insertNode(moves,newNode(m, sizeof(move))); // a possible move - add it to the list
				if (capture) break;
				if (singleMove) break; // continue to new direction
			}
		}
	}
	else 
	{
		int* directions;
		if (type == WHITE_P)
		{
			directions = WHITE_P_DIRECTIONS;
		}
		else
		{
			directions = BLACK_P_DIRECTIONS;
		}
		int ind = posToInd(p);
		move *m;
		if (board[ind + directions[0]] == EMPTY){// make a step forward
			m = newMove(p, indToPos(ind + directions[0]), 0);
			memcpy(newBoard, board, 120 * sizeof(char));
			makeMove(m, newBoard);
			if (!isCheck(newBoard, color))
			{
				insertNode(moves, newNode(m, sizeof(move)));
			}	
			else{
				free(m);
			}
		}
		for (int i = 1; i < 3; i++)
		{
			if (colorOfLoc(board, ind + directions[i]) == 1 - colorOfLoc(board, posToInd(p))) {// capture
				m = newMove(p, indToPos(ind + directions[i]), 1);
				memcpy(newBoard, board, 120 * sizeof(char));
				makeMove(m, newBoard);
				if (!isCheck(newBoard, color))
				{
					insertNode(moves, newNode(m, sizeof(move)));
				}
				else
				{
					free(m);
				}
			}
		}
	}
	free(newBoard);
}

/*returns a list containing all the possible moves by the given player*/
linkedList* getMoves(char* board, int player){
	linkedList* moves = newLinkedList();
	pos searcher;
	for (int x = 'a'; x < 'i'; x++)
	{
		searcher.x = x;
		for (int y = 1; y < 9; y++)
		{
			searcher.y = y;
			if (colorOfLoc(board,posToInd(searcher)) == player)
			{
				genMoves(searcher,moves,board);
			}
		}
	}
	return moves;
}
/* assumes that the given player is in check,
remove all moves that keeps in him check*/
void makeMove(move* m, char* board){
	char type = board[posToInd(m->origin)];
	board[posToInd(m->origin)] = EMPTY;
	board[posToInd(m->dest)] = type;
}

void updateMoveList(linkedList* moves, int player, char* board){
	listNode *prev = NULL;
	listNode *curr = moves->first;
	while (curr != NULL)
	{
		char* newBoard = (char*)malloc(120 * sizeof(char));
		memcpy(newBoard, board, 120 * sizeof(char));
		makeMove((move*)curr->data, newBoard);
		if (isCheck(newBoard,player))
		{
			curr = removeNode(moves, prev, curr);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
		free(newBoard);
	}
}

int score(char* board, int player){
	linkedList *moves = getMoves(board, 1 - player);
	if (moves->first == NULL){ // no possible moves for other player
		if (isCheck(board, 1 - player)){ // check
			freeList(moves);
			return 500; // the player wins
		}
		else // not in check
		{
			freeList(moves);
			return -0; // tie
		}
	}
	if (moves->first == NULL){ // no possible moves
		if (isCheck(board, player)){ // check
			freeList(moves);
			return -500; // the player lost
		}
		else // not in check
		{
			freeList(moves);
			return -0; // tie
		}
	}
	int score = 0;
	pos searcher;
	for (int x = 'a'; x < 'i'; x++)
	{
		searcher.x = x;
		for (int y = 1; y < 9; y++)
		{
			searcher.y = y;
			char type = board[posToInd(searcher)];
			int color = colorOfLoc(board, posToInd(searcher));
			if (type == EMPTY)
			{
				continue;
			}
			if (type == WHITE_P || type == BLACK_P)
				score += 1 * (2 * color - 1);
			else if (type == WHITE_N || type == BLACK_N || type == WHITE_B || type == BLACK_B)
				score += 3 * (2 * color - 1);
			else if (type == WHITE_R || type == BLACK_R)
				score += 5 * (2 * color - 1);
			else if (type == WHITE_Q || type == BLACK_Q)
				score += 9 * (2 * color - 1);
			else
				score += 400 * (2 * color - 1);
		}
	}

	freeList(moves);
	return player==WHITE ? score : -1 * score;
}

/*promote the given pawn to the given type*/
/*void promote(char* board, char type){
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
}*/

/*returns 1 if the given player threaten the given position
otherwise, return 0*/
int isThreat(pos p, char* board, int player){
	int ind = posToInd(p);
	char rook = player == WHITE ? WHITE_R : BLACK_R;
	char bishop = player == WHITE ? WHITE_B : BLACK_B;
	char knight = player == WHITE ? WHITE_N : BLACK_N;
	char queen = player == WHITE ? WHITE_Q : BLACK_Q;
	char king = player == WHITE ? WHITE_K : BLACK_K;
	char pawn = player == WHITE ? WHITE_P : BLACK_P;
	int *directions = R_DIRECTIONS; // rook threat
	for (int i = 0; i < 4; i++)
	{
		char flag = 1;
		ind = posToInd(p);
		while (1)
		{
			ind += directions[i];
			if (colorOfLoc(board, ind) == 1 - player || board[ind] == OUT_OF_BOARD)
			{
				break;
			}
			if (board[ind] == rook || board[ind] == queen || (board[i] == king && flag))
			{
				return 1;
			}
			if (board[ind] == EMPTY)
			{
				flag = 0;
				continue;
			}
			else
			{
				break;
			}
		}
	}
	ind = posToInd(p);
	directions = B_DIRECTIONS; // bishop threat
	for (int i = 0; i < 4; i++)
	{
		char flag = 1;
		ind = posToInd(p);
		while (1)
		{
			ind += directions[i];
			if (colorOfLoc(board, ind) == 1 - player || board[ind] == OUT_OF_BOARD)
			{
				break;
			}
			if (board[ind] == bishop || board[ind] == queen || (board[i] == king && flag))
			{
				return 1;
			}
			if (board[ind] == EMPTY)
			{
				flag = 0;
				continue;
			}
			else
			{
				break;
			}
		}
	}
	directions = N_DIRECTIONS; // knight threat
	for (int i = 0; i < 8; i++)
	{
		ind = posToInd(p);
		ind += directions[i];
		if (board[ind] == knight)
		{
			return 1;
		}
	}
	if (player == WHITE)
	{
		directions = BLACK_P_DIRECTIONS;
	}
	else
	{
		directions = WHITE_P_DIRECTIONS;
	}
	for (int i = 1; i < 3; i++)
	{
		ind = posToInd(p);
		if (board[ind + directions[i]] == pawn){
			return 1;
		}
	}
	return 0;
}

/*returns 1 if the given player is in check in the given board*/
int isCheck(char* board, int player){
	char type = player==WHITE ? WHITE_K : BLACK_K;
	pos kingPos = { -1, -1 };
	pos searcher;
	for (int x = 'a'; x < 'i'; x++)
	{
		searcher.x = x;
		for (int y = 1; y < 9; y++)
		{
			searcher.y = y;
			if (board[posToInd(searcher)] == type)
			{
				kingPos = searcher;
			}
		}
	}
	return isThreat(kingPos, board, 1 - player);
}

/*returns 1 if the given player has a possible move*/
int hasMoves(char* board, int player){
	return (getMoves(board, player)->first != NULL);
}