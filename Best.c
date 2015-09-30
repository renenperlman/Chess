#include "Best.h"

int pawnScore = 100;
int knightScore = 320;
int bishopScore = 330;
int rookScore = 500;
int queenScore = 900;
int kingScore = 20000;


int pawnTable[64] = {  0,  0,  0,  0,  0,  0,  0,  0,
					  50, 50, 50, 50, 50, 50, 50, 50,
					  10, 10, 20, 30, 30, 20, 10, 10,
					   5,  5, 10, 25, 25, 10,  5,  5,
					   0,  0,  0, 20, 20,  0,  0,  0,
					   5, -5, -10, 0,  0, -10, -5, 5,
				       5, 10, 10, -20, -20, 10, 10, 5,
					   0,  0,  0,  0,  0,  0,  0,  0 };

int knightTable[64] = { -50, -40, -30, -30, -30, -30, -40, -50,
-40, -20, 0, 0, 0, 0, -20, -40,
-30, 0, 10, 15, 15, 10, 0, -30,
-30, 5, 15, 20, 20, 15, 5, -30,
-30, 0, 15, 20, 20, 15, 0, -30,
-30, 5, 10, 15, 15, 10, 5, -30,
-40, -20, 0, 5, 5, 0, -20, -40,
-50, -40, -30, -30, -30, -30, -40, -50 };

int bishoptTable[64] = { -20, -10, -10, -10, -10, -10, -10, -20,
-10, 0, 0, 0, 0, 0, 0, -10,
-10, 0, 5, 10, 10, 5, 0, -10,
-10, 5, 5, 10, 10, 5, 5, -10,
-10, 0, 10, 10, 10, 10, 0, -10,
-10, 10, 10, 10, 10, 10, 10, -10,
-10, 5, 0, 0, 0, 0, 5, -10,
-20, -10, -10, -10, -10, -10, -10, -20 };

int rookTable[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
5, 10, 10, 10, 10, 10, 10, 5,
-5, 0, 0, 0, 0, 0, 0, -5,
-5, 0, 0, 0, 0, 0, 0, -5,
-5, 0, 0, 0, 0, 0, 0, -5,
-5, 0, 0, 0, 0, 0, 0, -5,
-5, 0, 0, 0, 0, 0, 0, -5,
0, 0, 0, 5, 5, 0, 0, 0 };

int queenTable[64] = { -20, -10, -10, -5, -5, -10, -10, -20,
-10, 0, 0, 0, 0, 0, 0, -10,
-10, 0, 5, 5, 5, 5, 0, -10,
-5, 0, 5, 5, 5, 5, 0, -5,
0, 0, 5, 5, 5, 5, 0, -5,
-10, 5, 5, 5, 5, 5, 0, -10,
-10, 0, 5, 0, 0, 0, 0, -10,
-20, -10, -10, -5, -5, -10, -10, -20 };

int board64[64];

void initBestTable(){
	int cnt = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++){
			board64[7 * (8 - i) + j] = cnt++;
		}
	}
}

int posToBoard64(pos p){
	return board64[7 * (p.y - 1) + p.x - 'a'];
}

int scoreBest(char* board,int player){
	linkedList *moves = getMoves(board, BLACK);
	if (moves->first == NULL){ // no possible moves for other player
		if (isCheck(board, BLACK)){ // check
			freeList(moves);
			return 2*kingScore; // the player wins
		}
		else // not in check
		{
			freeList(moves);
			return -0; // tie
		}
	}
	freeList(moves);
	moves = getMoves(board, WHITE);
	if (moves->first == NULL){ // no possible moves
		if (isCheck(board, WHITE)){ // check
			freeList(moves);
			return -2 * kingScore; // the player lost
		}
		else // not in check
		{
			freeList(moves);
			return -0; // tie
		}
	}
	freeList(moves);
	int score = 0;
	pos p;
	for (char x = 'a'; x <= 'h'; x++)
	{
		p.x = x;
		for (int y = 1; y <= 8; y++)
		{
			p.y = y;
			int ind = posToInd(p);
			char type = tolower(board[ind]);
			if (type == EMPTY)
			{
				continue;
			}
			int color = colorOfLoc(board, ind);
			int ind64 = color == WHITE ? posToBoard64(p) : 64 - posToBoard64(p);
			if (type == 'm')
			{
				score += pawnScore * (2 * color - 1);
				score += pawnTable[ind64];
			}
			else if (type == 'n'){
				score += knightScore * (2 * color - 1);
				score += knightTable[ind64];
			}
			else if (type == 'b'){
				score += bishopScore * (2 * color - 1);
				score += bishoptTable[ind64];
			}
			else if (type == 'r'){
				score += rookScore * (2 * color - 1);
				score += rookTable[ind64];
			}
			else if (type == 'q'){
				score += queenScore * (2 * color - 1);
				score += queenTable[ind64];
			}
		}
	}
	return score * (2 * player - 1);
}
