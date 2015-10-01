#include "Minimax.h"


int alphabeta(char* board, int *alpha, int *beta, int depth, int isMax ,int firstCall, int **scores, int (score)(char*,int)){
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
			if (scores == NULL)
			{
				print_message("Error: standard function calloc has failed");
				print_message("Terminating program");
				exit(1);
			}
		}
		int v = INT_MIN, i = 0, v2;
		node = moves->first;
		if (node == NULL)
		{
			free(moves);
			return score(board, nextPlayer);
		}
		while (node!=NULL)
		{
			int tempAlpha = *alpha, tempBeta = *beta;
			char otherType = board[posToInd(((move*)node->data)->dest)];
			makeMove((move*)node->data, board);
			v2 = alphabeta(board, &tempAlpha, &tempBeta, depth - 1, 0, 0, scores, score);
			unmakeMove(node->data, board, otherType);
			if (firstCall){
				(*scores)[i] = v2;
				i++;
			}
			node = removeNode(moves, NULL, node);
			v = max(v, v2);
			*alpha = max(v, tempAlpha);
			if (*beta < *alpha || *beta == *alpha)
			{
				break;
			}
		}
		freeList(moves);
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
		while (node != NULL)
		{
			int tempAlpha = *alpha, tempBeta = *beta;
			char otherType = board[posToInd(((move*)node->data)->dest)];
			makeMove((move*)node->data, board);
			v = min(v, alphabeta(board, &tempAlpha, &tempBeta, depth - 1, 1, 0, scores, score));
			unmakeMove(node->data, board, otherType);
			node = removeNode(moves, NULL, node);
			*beta = min(v, tempBeta);
			if (*beta < *alpha || *beta == *alpha)
			{
				break;
			}
		}
		freeList(moves);
		return v;
	}
}


linkedList *getBestMoves(char *board, int player, int depth, int (score)(char*, int)){
	linkedList *bestMoves = newLinkedList();
	int alpha = INT_MIN, beta = INT_MAX;
	int *scores = NULL, best = alphabeta(board, &alpha, &beta, depth, 1, 1, &scores,score);
	linkedList* moves = getMoves(board, player);
	int cnt = 0;
	listNode *node = moves->first;
	while (node != NULL)
	{
		if (scores[cnt] == best)
		{
			move *m = node->data;
			char otherType = board[posToInd(m->dest)];
			makeMove(m, board);
			alpha = INT_MIN, beta = INT_MAX;
			if (alphabeta(board, &alpha, &beta, depth - 1, 0, 0, &scores, score) == best)
			{
				insertNode(bestMoves, cloneNode(node));
			}
			unmakeMove(m, board, otherType);
		}
		node = node->next;
		cnt++;
	}
	freeList(moves);
	free(scores);
	return bestMoves;
}

