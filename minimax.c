#include "Minimax.h"

int alphabeta(char* board, piece* pieces, int player, int alpha, int beta, int depth, int isMax, int (score)(piece*, char*, int),
	linkedList* (getMoves)(piece*, char[], int), void (makeMove)(move*, piece*, char[])){
	linkedList* moves = getMoves(pieces, board, player);
	if (depth == 0 || moves->first==NULL)
	{
		freeList(moves);
		return score(pieces, board, player);
	}
	
	if (isMax)
	{
		int v = INT_MIN;
		listNode* node = moves->first;
		while (node!=NULL)
		{
			char* newBoard = (char*)cloneData(board, 120 * sizeof(char));
			piece* newPieces = (piece*)cloneData(pieces, 16 * sizeof(piece));
			makeMove((move*)node->data, newPieces, newBoard);
			v = max(v, alphabeta(newBoard, newPieces, 1 - player, alpha, beta, depth - 1, 0, score, getMoves, makeMove));
			free(newBoard);
			free(newPieces);
			alpha = max(v, alpha);
			if (beta < alpha || beta == alpha)
			{
				break;
			}
		}
		freeList(moves);
		return v;
	}
	else
	{
		int v = INT_MAX;
		listNode* node = moves->first;
		while (node != NULL)
		{
			char* newBoard = (char*)cloneData(board, 120 * sizeof(char));
			piece* newPieces = (piece*)cloneData(pieces, 16 * sizeof(piece));
			makeMove((move*)node->data, newPieces, newBoard);
			v = min(v, alphabeta(newBoard, newPieces, 1 - player, alpha, beta, depth - 1, 1, score, getMoves, makeMove));
			free(newBoard);
			free(newPieces);
			beta = max(v, alpha);
			if (beta < alpha || beta == alpha)
			{
				break;
			}
		}
		freeList(moves);
		return v;
	}
}