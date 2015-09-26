#include "Console.h"


char board[120] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, 'R','N','B','Q','K','B','N','R',-1,
					-1, 'M','M','M','M','M','M','M','M',-1,
					-1, ' ',' ',' ',' ',' ',' ',' ',' ',-1,
					-1, ' ',' ',' ',' ',' ',' ',' ',' ',-1,
					-1, ' ',' ',' ',' ',' ',' ',' ',' ',-1,
					-1, ' ',' ',' ',' ',' ',' ',' ',' ',-1,
					-1, 'm','m','m','m','m','m','m','m',-1,
					-1, 'r','n','b','q','k','b','n','r',-1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


/*clears the board and the pieces*/
void clear(){
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[mailbox[8 * j + i]] = EMPTY;
		}
	}
}

void print_line(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

void print_board(char* board)
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", board[mailbox[8 * j + i]]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

char * getInput(FILE * fp, size_t size){
	char *str;
	int ch;
	size_t len = 0;
	if ((str = (char *)malloc(sizeof(char)*((int)size))) == NULL){
		printf("Memory allocation error!");
		exit(1);
	}
	while (EOF != (ch = fgetc(fp)) && ch != '\n')
	{
		str[len++] = ch;
		if (len == size)
		{
			if ((str = realloc(str, sizeof(char)*(size += 128))) == NULL){
				printf("Memory allocation error!");
				exit(1);
			}
		}
	}
	str[len++] = '\0';
	return realloc(str, sizeof(char)*len);
}

move *parseMove(char* input, int player){
	pos start = { input[6], input[8] - '0' };
	pos dest = { input[15], input[17] - '0' };
	char prom = 0;
	if (input[19])
	{
		prom = input[20] == 'k' ? 'n' : input[20];
	}
	if (!isLegalPos(start) || !isLegalPos(dest)){
		print_message(WRONG_POSITION);
		return NULL;
	}
	if (colorOfLoc(board,posToInd(start))!=player)
	{
		print_message(WRONG_PIECE);
		return NULL;
	}
	linkedList *moves = newLinkedList();
	genMoves(start, moves, board); // checking if legal move
	listNode *node = moves->first;
	move *m = NULL;
	while (node != NULL) // searching in possible moves
	{
		if (compPos(((move*)node->data)->dest, dest))
		{
			if (((move*)node->data)->promType) // promotion move
			{
				if (!prom && ((move*)node->data)->promType == 'q'){ // prom not given by user
					m = (move*)cloneData(node->data, sizeof(move)); // using queen prom by default
				}
				else if (((move*)node->data)->promType == prom){// the correct prom
					m = (move*)cloneData(node->data, sizeof(move));
				}
			}
			else
			{
				m = (move*)cloneData(node->data, sizeof(move)); // legal move
			}	
		}
		node = node->next;
	}
	freeList(moves);
	if (m == NULL) // illegal move
	{
		print_message(ILLEGAL_MOVE);
		return NULL;
	}
	return m;
}

int endGamePrint(int player){
	int check = isCheck(board, 1 - player);
	if (hasMoves( board, 1 - player) == 0) // other player cant move
	{
		if (check){ // mate
			if (player == WHITE)
			{
				print_message(WHITE_WINS);
				return 1;
			}
			else
			{
				print_message(BLACK_WINS);
				return 1;
			}
		}
		else{
			print_message(TIE);
			return 1;
		}
	}
	if (check)
	{
		print_message(CHECK);
	}
	return 0;
}

/*the main loop for the game setting mode
returns 0 if user chose to start the game
returns 1 if the user chose to quit*/
int settingMode(){
	FILE *fp = stdin;
	char input[51];
	int ch;
	print_message(ENTER_SETTINGS);
	while (1)
	{
		int cnt = 0;
		while (EOF != (ch = fgetc(fp)) && ch != '\n'){
			input[cnt++] = ch;
		}
		input[cnt++] = '\0';
		if (strncmp(input, "game_mode", 9) == 0){ // game mode
			if (input[10] == '1' && input[11] == '\0')
			{
				gameMode = input[10] - '0';
				print_message(TWO_PLAYERS_GAME_MODE);
			}
			else if (input[10] == '2' && input[11] == '\0')
			{
				gameMode = input[10] - '0';
				print_message(PLAYER_VS_AI_GAME_MODE);
			}
			else{
				print_message(WRONG_GAME_MODE);
			}
		}
		else if (strncmp(input, "difficulty", 10) == 0){ // difficulty
			if (gameMode != 2){
				print_message(ILLEGAL_COMMAND);
			}
			else if (strncmp(input, "difficulty best", 15) == 0){
				depth = 5;
			}
			else if (input[17] - '0' > 4 || input[17] - '0' < 0)
			{
				print_message(WRONG_MINIMAX_DEPTH);
			}
			else{
				depth = input[17] - '0';
			}
		}
		else if (strncmp(input, "user_color", 10) == 0){ // user color
			if (gameMode != 2){
				print_message(ILLEGAL_COMMAND);
			}
			else
			{
				userColor = input[11] == 'w' ? WHITE : BLACK;
			}
		}
		else if (strncmp(input, "load", 4) == 0){ // load
			clear();
			FILE * fp = fopen(input + 5, "r");
			if (fp == NULL)
			{
				print_message(WRONG_FILE_NAME);
				continue;
			}
			loadGame(fp, board);
			print_board(board);
			fclose(fp);
		}
		else if (strncmp(input, "clear", 5) == 0){
			clear();
		}
		else if (strncmp(input, "next_player", 11) == 0){
			nextPlayer = input[12] == 'w' ? WHITE : BLACK;
		}
		else if (strncmp(input, "rm", 2) == 0){ // remove
			pos p = { input[4], input[6] - '0' };
			if (!isLegalPos(p))
			{
				print_message(WRONG_POSITION);
				continue;
			}
			board[posToInd(p)] = EMPTY;
		}
		else if (strncmp(input, "set", 3) == 0){ // set
			pos p = { input[5], input[7] - '0' };	// get the position from the input
			int color = input[10] == 'w' ? WHITE : BLACK;
			char type = (input[16] == 'k' && input[17] == 'n') ? 'n' : input[16]; // get the type from the input
			type = type == 'p' ? 'm' : type;
			type = color == WHITE ? (char)tolower(type) : (char)toupper(type); // change case according to the color
			set(board, p, type, color);
		}
		else if ((strncmp(input, "print", 5) == 0)){
			print_board(board);
		}
		else if (strncmp(input, "start", 5) == 0){ // start
			int flagW = 0, flagB = 0;
			pos kingPos;
			for (int x = 'a'; x < 'i'; x++)
			{
				kingPos.x = x;
				for (int y = 1; y < 9; y++)
				{
					kingPos.y = y;
					if (board[posToInd(kingPos)] == WHITE_K)
					{
						flagW = 1;
					}
					if (board[posToInd(kingPos)] == BLACK_K)
					{
						flagB = 1;
					}
				}
			}
			if (!flagB || !flagW) // no kings
			{
				print_message(WROND_BOARD_INITIALIZATION);
				continue;
			}
			linkedList* moves = getMoves(board, nextPlayer);
			if (moves->first == NULL) // the first player has no possible moves
			{
				print_message(WROND_BOARD_INITIALIZATION);
				continue;
			}
			else if (isCheck(board,1 - nextPlayer)) // the other player is in check 
				// (that means that the next player can move and capture the king)
			{
				print_message(WROND_BOARD_INITIALIZATION);
				continue;
			}
			freeList(moves);
			return 0; // can start the game

		}
		else if (strncmp(input, "quit", 4) == 0){ // quit
			return 1;
		}
		else
		{
			print_message(ILLEGAL_COMMAND);
		}
	}
	return 0;
}

/* the main loop for user's turn
returns 1 if the game is over, otherwise returns 0*/
int userTurn(int player){
	char* color = player == WHITE ? "White" : "Black";
	FILE *fp = stdin;
	char input[51];
	int ch;
	while (1)
	{
		print_message(color);
		print_message(" player - enter your move: \n");
		int cnt = 0;
		while (EOF != (ch = fgetc(fp)) && ch != '\n'){
			input[cnt++] = ch;
		}
		input[cnt++] = '\0';
		if (strncmp(input, "move", 4) == 0){ // move
			move *m = parseMove(input,player);
			if (m == NULL) // invalid move
			{
				continue;
			}
			makeMove(m, board);
			free(m);
			print_board(board);
			return endGamePrint(player);
		}
		if (strncmp(input, "get_moves", 9) == 0){ // get moves
			pos position = { input[11], input[13] - '0' };
			if (!isLegalPos(position))
			{
				print_message(WRONG_POSITION);
				continue;
			}
			if (colorOfLoc(board,posToInd(position))!=player)
			{
				print_message(WRONG_PIECE);
				continue;
			}
			linkedList *moves = newLinkedList();
			genMoves(position, moves, board);
			if (isCheck(board,player))
			{
				updateMoveList(moves, player, board);
			}
			printMoves(moves);
			freeList(moves);
		}
		else if (strncmp(input, "get_best_moves",14) == 0) // get best moves
		{
			linkedList *bestMoves = getBestMoves(board, player, input[15] - '0'); // !!! change this to include best difficulty !!!
			//printf("prune count = %d\n", pruneCount);
			printMoves(bestMoves);
			freeList(bestMoves);
		}
		else if (strncmp(input, "get_score", 9) == 0) // get scores
		{
			int d = input[10] - '1';
			char* newBoard = (char*)malloc(120 * sizeof(char));
			memcpy(newBoard, board, 120 * sizeof(char));
			move *m = parseMove(input + 12,player);
			makeMove(m, newBoard);
			int *scores = NULL, score = alphabeta(newBoard, INT_MIN, INT_MAX, d, 0, 0, &scores);
			free(scores);
			free(newBoard);
			free(m);
			printf("%d\n", score);
		}
		else if (strncmp(input, "save", 4) == 0){ // save
			FILE * fp = fopen(input + 5, "w");
			if (fp == NULL)
			{
				print_message(WRONG_FILE_NAME);
				continue;
			}
			saveGame(fp, board);
			fclose(fp);
		}
		else if (strncmp(input, "quit", 4) == 0){ // quit
			return 1;
		}
		else
		{
			print_message(ILLEGAL_COMMAND);
		}
	}
	return 0;
}


int compTurn(){
	linkedList *bestMoves = getBestMoves(board, 1 - userColor, depth);
	move *m = bestMoves->first->data;
	print_message("Computer: move ");
	printMove(m);
	makeMove(m, board);
	freeList(bestMoves);
	print_board(board);
	return endGamePrint(1 - userColor);
}

int consoleMode(){
	initMailBox120();
	print_board(board);
	int n = settingMode();
	while (n==0)
	{
		if (gameMode == 2)
		{
			if (nextPlayer == userColor)
			{
				n = userTurn(userColor);
			}
			else{
				n = compTurn();
			}
			nextPlayer = 1 - nextPlayer;
		}
		else
		{
			n = userTurn(nextPlayer);
			nextPlayer = 1 - nextPlayer;
			userColor = 1 - userColor;
		}
	}
	return 1;
}