#include "Minimax.h"


int alphabeta(char* board, int alpha, int beta, int depth, int isMax ,int firstCall, int **scores){
	if (depth == 0)
	{
		return score(board, nextPlayer);
	}
	if (isMax)
	{
		linkedList* moves = getMoves(board, nextPlayer);
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
			return score(board, nextPlayer);
		}
		char* newBoard = (char*)malloc(120 * sizeof(char));
		while (node!=NULL)
		{
			memcpy(newBoard, board, 120 * sizeof(char));
			makeMove((move*)node->data, newBoard);
			v2 = alphabeta(newBoard, alpha, beta, depth - 1, 0, 0, scores);
			if (firstCall){
				(*scores)[i] = v2;
				i++;
			}
			node = removeNode(moves, NULL, node);

			v = max(v, v2);
			alpha = max(v, alpha);
			if (beta < alpha || beta == alpha)
			{
				break;
			}
		}
		freeList(moves);
		free(newBoard);
		return v;
	}
	else
	{
		linkedList* moves = getMoves(board, 1-nextPlayer);
		int v = INT_MAX;
		listNode* node = moves->first;
		if (node == NULL)
		{
			free(moves);
			return score(board, nextPlayer);
		}
		char* newBoard = (char*)malloc(120 * sizeof(char));
		while (node != NULL)
		{
			memcpy(newBoard, board, 120 * sizeof(char));
			makeMove((move*)node->data, newBoard);
			v = min(v, alphabeta(newBoard, alpha, beta, depth - 1, 1, 0, scores));
			node = removeNode(moves, NULL, node);
			beta = min(v, beta);
			if (beta < alpha || beta == alpha)
			{
				break;
			}
		}
		free(newBoard);
		freeList(moves);
		return v;
	}
}


linkedList *getBestMoves(char *board, int player, int depth){
	linkedList *bestMoves = newLinkedList();
	int *scores = NULL, best = alphabeta(board, INT_MIN, INT_MAX, depth, 1, 1, &scores);
	linkedList* moves = getMoves(board, player);
	int cnt = 0;
	listNode *node = moves->first;
	char* newBoard = (char*)malloc(120 * sizeof(char));
	while (node != NULL)
	{
		if (scores[cnt] == best)
		{
			move *m = node->data;
			memcpy(newBoard, board, 120 * sizeof(char));
			makeMove(m, newBoard);
			if (alphabeta(newBoard, INT_MIN, INT_MAX, depth - 1, 0, 0, &scores) == best)
			{
				insertNode(bestMoves, cloneNode(node));
			}
			
		}
		node = node->next;
		cnt++;
	}
	freeList(moves);
	free(scores);
	free(newBoard);
	return bestMoves;
}

