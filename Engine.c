#include "Engine.h"


/*premade arrays showing the possible move directions
for each piece with respect to the mailbox indexing*/
int WHITE_P_DIRECTIONS[3] = { -10, -9, -11 };
int BLACK_P_DIRECTIONS[3] = { 10, 9, 11 };
int N_DIRECTIONS[8] = { -21, -19, -12, -8, 8, 12, 19, 21 };
int B_DIRECTIONS[4] = { -11, -9, 9, 11 };
int R_DIRECTIONS[4] = { -10, -1, 1, 10 };
int QnK_DIRECTIONS[8] = { -11, -10, -9, -1, 1, 9, 10, 11 };


/* Return the color of the piece in the given index on the give board
1 - white
0 - black
-1 - empty or out of board*/
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
	int color = colorOfLoc(board, posToInd(p));
	if (type != WHITE_P && type != BLACK_P) // not a pawn
	{
		int numOfDirections = (type == WHITE_R || type == BLACK_R 
			|| type == WHITE_B || type == BLACK_B) ? 4 : 8; // the number of possible move directions
		int* directions;
		char singleMove; //  a flag if the piece can move only once in a direction
		if (type == WHITE_R || type == BLACK_R) //setting the directions of the piece
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
		for (int i = 0; i < numOfDirections; i++) // making moves in each direction
		{
			int ind = posToInd(p);
			while (1)
			{
				ind += directions[i];
				if (board[ind] == OUT_OF_BOARD) break; // out of bound
				if (colorOfLoc(board, ind) == color) break; // same piece color
				int capture = board[ind] == EMPTY ? 0 : 1;
				move *m = newMove(p, indToPos(ind),0);
				char otherType = board[ind];
				makeMove(m, board);
				if (isCheck(board,color)) // if the moves exposes the kind, don't add it
				{
					unmakeMove(m, board, otherType); // revert the move
					free(m);
					if (singleMove || capture) // if the given direction has been exhuasted
					{
						break;
					}
					continue;
				}
				unmakeMove(m, board, otherType);
				insertNode(moves,newNode(m, sizeof(move))); // a possible move - add it to the list
				if (capture) break; // direction exhuasted
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
			m = newMove(p, indToPos(ind + directions[0]),0);
			char otherType = board[ind + directions[0]];
			makeMove(m, board);
			if (!isCheck(board, color)) // the move doesn't expose the king
			{
				if ((color == WHITE && m->dest.y == 8) || (color == BLACK && m->dest.y == 1)) // promotion
				{
					unmakeMove(m, board, otherType);
					free(m);
					m = newMove(p, indToPos(ind + directions[0]), 'q');
					insertNode(moves, newNode(m, sizeof(move)));
					m = newMove(p, indToPos(ind + directions[0]), 'b');
					insertNode(moves, newNode(m, sizeof(move)));
					m = newMove(p, indToPos(ind + directions[0]), 'r');
					insertNode(moves, newNode(m, sizeof(move)));
					m = newMove(p, indToPos(ind + directions[0]), 'n');
					insertNode(moves, newNode(m, sizeof(move)));
				}
				else{ // regular move
					unmakeMove(m, board, otherType);
					insertNode(moves, newNode(m, sizeof(move)));
				}
			}	
			else{ // illegal move
				unmakeMove(m, board, otherType);
				free(m);
			}
		}
		for (int i = 1; i < 3; i++)
		{
			if (colorOfLoc(board, ind + directions[i]) == 1 - colorOfLoc(board, posToInd(p))) {// capture
				m = newMove(p, indToPos(ind + directions[i]),0);
				char otherType = board[ind + directions[i]];
				makeMove(m, board);
				if (!isCheck(board, color))
				{
					if ((color == WHITE && m->dest.y == 8) || (color == BLACK && m->dest.y == 1))
					{
						unmakeMove(m, board, otherType);
						free(m);
						m = newMove(p, indToPos(ind + directions[i]), 'q');
						insertNode(moves, newNode(m, sizeof(move)));
						m = newMove(p, indToPos(ind + directions[i]), 'b');
						insertNode(moves, newNode(m, sizeof(move)));
						m = newMove(p, indToPos(ind + directions[i]), 'r');
						insertNode(moves, newNode(m, sizeof(move)));
						m = newMove(p, indToPos(ind + directions[i]), 'n');
						insertNode(moves, newNode(m, sizeof(move)));
					}
					else{
						unmakeMove(m, board, otherType);
						insertNode(moves, newNode(m, sizeof(move)));
					}
				}
				else
				{
					unmakeMove(m, board, otherType);
					free(m);
				}
			}
		}
	}
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



void makeMove(move* m, char* board){
	char type;
	if (m->promType == 0)
	{
		type = board[posToInd(m->origin)];
	}
	else
	{
		type = colorOfLoc(board, posToInd(m->origin)) == WHITE ? m->promType : toupper(m->promType);
	}
	board[posToInd(m->origin)] = EMPTY;
	board[posToInd(m->dest)] = type;
}

/*unmaked the given move
otherType is the state of the board before
the piece moved into that tile*/
void unmakeMove(move *m, char *board,char otherType){
	char type;
	if (m->promType == 0)
	{
		type = board[posToInd(m->dest)];
	}
	else
	{
		type = colorOfLoc(board, posToInd(m->dest)) == WHITE ? WHITE_P : BLACK_P;
	}
	board[posToInd(m->dest)] = otherType;
	board[posToInd(m->origin)] = type;
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
			return -400; // tie
		}
	}
	freeList(moves);
	moves = getMoves(board, player);
	if (moves->first == NULL){ // no possible moves
		if (isCheck(board, player)){ // check
			freeList(moves);
			return -500; // the player lost
		}
		else // not in check
		{
			freeList(moves);
			return -400; // tie
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
	pos kingPos;
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
	linkedList *moves = getMoves(board, player);
	int res = moves->first != NULL;
	freeList(moves);
	return res;
}