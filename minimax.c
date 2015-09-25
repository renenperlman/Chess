#include "Minimax.h"

pruneCount = 0;

int alphabeta(char* board, piece* pieces, int alpha, int beta, int depth, int isMax ,int firstCall, int **scores){
	if (depth == 0)
	{
		return score(pieces, board, nextPlayer);
	}
	if (isMax)
	{
		linkedList* moves = getMoves(pieces, board, nextPlayer, isCheck(pieces, board, nextPlayer));
		listNode *node;
		if (firstCall)
		{
			int cnt = 0;
			node = moves->first;
			while (node != NULL)
			{
				node = node->next;
				cnt++;
			}
			*scores = (int*)calloc(cnt, sizeof(int));
		}
		int v = INT_MIN, i = 0, v2;
		node = moves->first;
		if (node == NULL)
		{
			free(moves);
			return score(pieces, board, nextPlayer);
		}
		char* newBoard = (char*)malloc(120 * sizeof(char));
		piece* newPieces = (piece*)malloc(NUMOFPIECES * sizeof(piece));
		while (node!=NULL)
		{
			memcpy(newBoard, board, 120 * sizeof(char));
			memcpy(newPieces, pieces, NUMOFPIECES * sizeof(piece));
			makeMove((move*)node->data, newPieces, newBoard);
			v2 = alphabeta(newBoard, newPieces, alpha, beta, depth - 1, 0, 0, scores);
			if (firstCall){
				(*scores)[i] = v2;
				i++;
			}
			node = removeNode(moves, NULL, node, 1);
			v = max(v, v2);
			alpha = max(v, alpha);
			if (beta < alpha || beta == alpha)
			{
				pruneCount++;
				break;
			}
			//node = node->next;
		}
		freeList(moves,1);
		free(newBoard);
		free(newPieces);
		return v;
	}
	else
	{
		linkedList* moves = getMoves(pieces, board, 1-nextPlayer, isCheck(pieces, board, 1 - nextPlayer));
		int v = INT_MAX;
		listNode* node = moves->first;
		if (node == NULL)
		{
			free(moves);
			return score(pieces, board, nextPlayer);
		}
		char* newBoard = (char*)malloc(120 * sizeof(char));
		piece* newPieces = (piece*)malloc(NUMOFPIECES * sizeof(piece));
		while (node != NULL)
		{
			memcpy(newBoard, board, 120 * sizeof(char));
			memcpy(newPieces, pieces, NUMOFPIECES * sizeof(piece));
			makeMove((move*)node->data, newPieces, newBoard);
			v = min(v, alphabeta(newBoard, newPieces, alpha, beta, depth - 1, 1, 0, scores));
		/*	if (v == 500)
			{
				printMove(node->data);
			}*/
			node = removeNode(moves, NULL, node, 1);
			beta = min(v, beta);
			if (beta < alpha || beta == alpha)
			{
				pruneCount++;
				break;
			}
			//node = node->next;
		}
		free(newBoard);
		free(newPieces);
		freeList(moves, 1);
		return v;
	}
}

linkedList *getBestMoves(char *board, piece *pieces, int player, int depth){
	linkedList *bestMoves = newLinkedList();
	int *scores = NULL, best = alphabeta(board, pieces, INT_MIN, INT_MAX, depth, 1, 1, &scores);
	linkedList* moves = getMoves(pieces, board, player, isCheck(pieces, board, player));
	int cnt = 0;
	listNode *node = moves->first;
	while (node != NULL)
	{
		if (scores[cnt] == best)
		{
			insertNode(bestMoves, cloneNode(node));
		}
		node = node->next;
		cnt++;
	}
	freeList(moves,1);
	free(scores);
	return bestMoves;
}