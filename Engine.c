#include "DS.h";

/* Return the color of the piece in the given index on the give board
assumes that there is a piece in the location
0 - white, 1 - black*/
int colorOfLoc(char* board, int ind){
	if (board[ind] == WHITE_P || board[ind] == WHITE_B ||
		board[ind] == WHITE_K || board[ind] == WHITE_N ||
		board[ind] == WHITE_Q || board[ind] == WHITE_R) return 0;
	return 1;
}
void genMoves(piece *p, linkedList *moves, char* board){
	for (int i = 0; i < p->numOfDirections; i++)
	{
		int ind = posToInd(p->position);
		while (1)
		{
			ind += p->directions[i];
			if (board[ind] == OUT_OF_BOARD) break; 
			if (colorOfLoc(board, ind) == p->color) break;

		}
	}
}