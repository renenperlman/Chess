#include "Best.h"

int pawnScore = 100;
int knightScore = 320;
int bishopScore = 330;
int rookScore = 500;
int queenScore = 900;
int kingScore = 20000;

pos lastPosKingW = { 'e', 1 };
pos lastPosKingB = { 'e', 8 };

int isKingInBoard(char* board, int color){
	pos p = color == WHITE ? lastPosKingW : lastPosKingB;
	int ind = posToInd(p);
	char type = color == WHITE ? WHITE_K : BLACK_K;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (board[ind + 10 * i + j] == type){
				return 1;
			}
		}
	}
	return 0;
}

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
/*int sortedBoard[64] = { 21, 22, 23, 24, 25, 26, 27, 28,
38, 48, 58, 68, 78, 88, 98,
97, 96, 95, 94, 93, 92, 91,
81, 71, 61, 51, 41, 31,
32, 33, 34, 35, 36, 37,
47, 57, 67, 77, 87,
86, 85, 84, 83, 82,
72, 62, 52, 42,
43, 44, 45, 46,
56, 66, 76,
75, 74, 73,
63, 53,
54, 55,
65, 64 };*/

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

/*linkedList* getMovesSorted(char* board, int player){
	linkedList *moves = newLinkedList();
	for (int i = 0; i < 64; i++)
	{
		if (board[colorOfLoc(board, sortedBoard[i]) == player]){
			genMoves(indToPos(sortedBoard[i]), moves, board);
		}
	}
	return moves;
}*/

/*int pvs(char *board, int depth, int alpha, int beta, int color){
	if (depth == 0)
	{
		return scoreBest(board) * (2 * color - 1);
	}
	linkedList *moves = getMovesSorted(board, color);
	if (moves->first == NULL)
	{
		free(moves);
		return scoreBest(board) * (2 * color - 1);
	}
	char first = 1;
	int s;
	listNode *node = moves->first;
	char* newBoard = (char*)malloc(120 * sizeof(char));
	while (node != NULL)
	{
		memcpy(newBoard, board, 120 * sizeof(char));
		makeMove((move*)node->data, newBoard);
		if (first){
			s = -1 * pvs(newBoard, depth - 1, -1 * beta, -1 * alpha, 1 - color);
			first = 0;
		}
		else
		{
			s = -1 * pvs(newBoard, depth - 1, -1 * alpha - 1, -1 * alpha, 1 - color);
			if (alpha < s || s < beta)
			{
				s = -1 * pvs(newBoard, depth - 1, -1 * beta , -1 * s, 1 - color);
			}
		}
		free(newBoard);
		alpha = max(alpha, s);
		if (alpha >= beta)
		{
			break;
		}
		node = node->next;
	}
	freeList(moves);
	return alpha;
}

linkedList* getBestMovesBest(char* board, int player){
	int s = pvs(board, 4, INT_MIN, INT_MAX, player);
	linkedList *bestMoves = newLinkedList(), *moves = getMoves(board, player);
	listNode *node = moves->first;
	char* newBoard = (char*)malloc(120 * sizeof(char));
	while (node != NULL)
	{
		memcpy(newBoard, board, 120 * sizeof(char));
		makeMove((move*)node->data, newBoard);
		if (pvs(newBoard,3,INT_MIN,INT_MAX,1-player) >= s)
		{
			insertNode(bestMoves, cloneNode(node));
		}
		free(newBoard);
		node = node->next;
	}
	freeList(moves);
	return bestMoves;
}
*/