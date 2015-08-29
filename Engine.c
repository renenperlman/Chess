#include "Engine.h"

/* Return the color of the piece in the given index on the give board
assumes that there is a piece in the location
0 - white, 1 - black*/
int colorOfLoc(char* board, int ind){
	if (board[ind] == WHITE_P || board[ind] == WHITE_B ||
		board[ind] == WHITE_K || board[ind] == WHITE_N ||
		board[ind] == WHITE_Q || board[ind] == WHITE_R) return 0;
	return 1;
}

void genMoves(piece *p, linkedList *moves, char board[]){
	if (p->type!='p')
	{
		for (int i = 0; i < p->numOfDirections; i++)
		{
			int ind = posToInd(p->position);
			while (1)
			{
				ind += p->directions[i];
				if (board[ind] == OUT_OF_BOARD) break;
				if (colorOfLoc(board, ind) == p->color) break;
				insertNode((newMove(p, indToPos(ind)), sizeof(move)), moves); // a possible move - add it to the list
				if (p->singleMove) break; // continue to new direction
			}
		}
	}
	else 
	{
		int ind = posToInd(p->position);
		if (board[ind + p->directions[0]]==EMPTY) // make a step forward
			insertNode((newMove(p, indToPos(ind + p->directions[0])), sizeof(move)), moves);
		for (int i = 1; i < 2; i++)
		{
			if (colorOfLoc(board, ind + p->directions[i]) != p->color) // capture
				insertNode((newMove(p, indToPos(ind + p->directions[i])), sizeof(move)), moves);
		}
	}
}

int score(piece *pieces,int player){
	int score = 0;
	for (int i = 0; i < 16; i++)
	{
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
	return player ? score : -1 * score;
}

piece* newWhiteP(pos position ){
	piece *whiteP = (piece*)malloc(sizeof(piece));
	whiteP->color = 1;
	whiteP->directions = WHITE_P_DIRECTIONS;
	whiteP->moved = 0;
	whiteP->numOfDirections = 3;
	whiteP->position = position;
	whiteP->singleMove = 1;
	whiteP->type = 'p';
}

piece* newWhiteB();
piece* newWhiten();
piece* newWhiteR();
piece* newWhiteQ();
piece* newWhiteK();
piece* newBlackP();
piece* newBlackB();
piece* newBlackN();
piece* newBlackR();
piece* newBlackK();