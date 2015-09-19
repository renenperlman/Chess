#include "Minimax.h"

int alphabeta(char* board, piece* pieces, int player, int alpha, int beta, int depth, int isMax ,int firstCall, int **scores){
	linkedList* moves = getMoves(pieces, board, player);
	if (depth == 0 || moves->first==NULL)
	{
		freeList(moves,1);
		return isMax ? score(pieces, board, player) : score(pieces, board, 1 - player);
	}
	if (firstCall)
	{
		int cnt = 0;
		listNode *node = moves->first;
		while (node!=NULL)
		{
			node = node->next;
			cnt++;
		}
		*scores = (int*)malloc(cnt*sizeof(int));
	}
	if (isMax)
	{
		int v = INT_MIN, i = 0,v2;
		listNode* node = moves->first;
		while (node!=NULL)
		{
			char* newBoard = (char*)malloc(120 * sizeof(char));
			memcpy(newBoard, board, 120 * sizeof(char));
			piece* newPieces = (piece*)malloc(NUMOFPIECES * sizeof(piece));
			memcpy(newPieces, pieces, NUMOFPIECES * sizeof(piece));
			makeMove((move*)node->data, newPieces, newBoard);
			v2 = alphabeta(newBoard, newPieces, 1 - player, alpha, beta, depth - 1, 0, 0, scores);
			if (firstCall){
				(*scores)[i] = v2;
				i++;
			}
			//node = removeNode(moves, NULL, node, 1);
			v = max(v, v2);
			free(newBoard);
			free(newPieces);
			alpha = max(v, alpha);
			if (beta < alpha || beta == alpha)
			{
				break;
			}
			node = node->next;
		}
		freeList(moves,1);
		return v;
	}
	else
	{
		int v = INT_MAX;
		listNode* node = moves->first;
		while (node != NULL)
		{
			char* newBoard = (char*)malloc(120 * sizeof(char));
			memcpy(newBoard, board, 120 * sizeof(char));
			piece* newPieces = (piece*)malloc(NUMOFPIECES * sizeof(piece));
			memcpy(newPieces, pieces, NUMOFPIECES * sizeof(piece));
			makeMove((move*)node->data, newPieces, newBoard);
			v = min(v, alphabeta(newBoard, newPieces, 1 - player, alpha, beta, depth - 1, 1, 0, scores));
			free(newBoard);
			free(newPieces);
			//node = removeNode(moves, NULL, node, 1);
			beta = max(v, alpha);
			if (beta < alpha || beta == alpha)
			{
				break;
			}
			node = node->next;
		}
		freeList(moves, 1);
		return v;
	}
}

linkedList *getBestMoves(char *board, piece *pieces, int player, int depth){
	linkedList *bestMoves = newLinkedList();
	int *scores = NULL, best = alphabeta(board, pieces, player, INT_MIN, INT_MAX, depth, 1, 1, &scores);
	linkedList* moves = getMoves(pieces, board, player);
	int cnt = 0;
	listNode *node = moves->first;
	while (node != NULL)
	{
		node = node->next;
		cnt++;
	}
	node = moves->first;
	for (int i = 0; i < cnt; i++)
	{
		if (scores[i] == best)
		{
			insertNode(bestMoves, cloneNode(node));
		}
		node = node->next;
	}
	freeList(moves,1);
	free(scores);
	return bestMoves;
}