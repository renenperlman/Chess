#include "Console.h"

gameMode = 1;
depth = 1;
userColor = WHITE;
nextPlayer = WHITE;
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

piece pieces[NUMOFPIECES];

void initPieces(){
	int i = 0;
	for (; i < 8; i++)
	{
		pos position = { i + 'a', 2 };
		pieces[i] = *(newPiece('m', WHITE, position)); // init white pawns
		position.y = 7;
		pieces[i+16] = *newPiece('M', BLACK, position); // init black pawns
	}
	i = 8;
	pos position = { 'a', 1 };
	pieces[i] = *newPiece('r', WHITE, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('n', WHITE, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('b', WHITE, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('q', WHITE, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('k', WHITE, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('b', WHITE, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('n', WHITE, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('r', WHITE, position);
	position.x = 'a';
	position.y = 8;
	i += 9;
	pieces[i] = *newPiece('r', BLACK, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('n', BLACK, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('b', BLACK, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('q', BLACK, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('k', BLACK, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('b', BLACK, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('n', BLACK, position);
	position.x++;
	i++;
	pieces[i] = *newPiece('r', BLACK, position);
}



/*clears the board and the pieces*/
void clear(){
	for (int i = 0; i < NUMOFPIECES; i++)
	{
		pieces[i].captured = 1;
	}
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
	if (!isLegalPos(start) || !isLegalPos(dest)){
		print_message(WRONG_POSITION);
		return NULL;
	}
	piece *p = NULL;
	for (int i = 0; i < NUMOFPIECES; i++)
	{
		if (compPos(pieces[i].position, start) && pieces[i].captured == 0 && pieces[i].color == player)
		{
			p = pieces + i;
			break;
		}
	}
	if (p == NULL)
	{
		print_message(WRONG_PIECE);
		return NULL;
	}
	linkedList *moves = newLinkedList();
	genMoves(p, moves, board); // checking if legal move
	listNode *node = moves->first;
	move *m = NULL;
	while (node != NULL) // searching in possible moves
	{
		if (compPos(((move*)node->data)->dest, dest))
		{
			m = (move*)cloneData(node->data,sizeof(move)); // legal move
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
	int check = isCheck(pieces, board, 1 - player);
	if (hasMoves(pieces, board, 1 - player) == 0) // other player cant move
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
	while (1)
	{
		print_message(ENTER_SETTINGS);
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
			// !!! complete this !!!
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
			for (int i = 0; i < NUMOFPIECES; i++)
			{
				if (compPos(pieces[i].position, p) && pieces[i].captured == 0)
				{
					pieces[i].captured = 1;
					break;
				}
			}
			board[posToInd(p)] = EMPTY;
		}
		else if (strncmp(input, "set", 3) == 0){ // set
			pos p = { input[5], input[7] - '0' };	// get the position from the input
			if (!isLegalPos(p))	// illegal pos
			{
				print_message(WRONG_POSITION);
				continue;
			}
			for (int i = 0; i < NUMOFPIECES; i++)
			{
				if (compPos(pieces[i].position, p) && pieces[i].captured == 0) 
				{
					pieces[i].captured = 1;	// remove the piece from the pos
					break;
				}
			}
			int color = input[10] == 'w' ? WHITE : BLACK;
			int flag = 0;
			char type = (input[16] == 'k' && input[17] == 'n') ? 'n' : input[16]; // get the type from the input
			type = type == 'p' ? 'm' : type;
			type = color == WHITE ? (char)tolower(type) : (char)toupper(type); // change case according to the color
			for (int i = 0; i < NUMOFPIECES; i++) // search for a free piece of the given type
			{
				if (pieces[i].color == color && pieces[i].type == type && pieces[i].captured == 1)
				{
					pieces[i].position = p;
					board[posToInd(p)] = type;
					pieces[i].captured = 0;
					pieces[i].moved = 1;
					flag = 1;
					break;
				}
			}
			if (!flag){	// couldnt find a free piece of the given type
				print_message(NO_PIECE); // illegal board
			}
		}
		else if ((strncmp(input, "print", 5) == 0)){
			print_board(board);
		}
		else if (strncmp(input, "start", 5) == 0){ // start
			int flag = 0;
			for (int i = 0; i < NUMOFPIECES; i++)
			{
				if ((pieces[i].type == BLACK_K || pieces[i].type == WHITE_K) && pieces[i].captured == 1) // no kings on the board
				{
					print_message(WROND_BOARD_INITIALIZATION);
					flag = 1;
					break;
				}
			}
			if (!flag && getMoves(pieces,board,nextPlayer)->first == NULL) // the first player has no possible moves
			{
				print_message(WROND_BOARD_INITIALIZATION);
				flag = 1;
			}
			else if (!flag && isCheck(pieces,board,1 - nextPlayer)) // the other player is in check 
				// (that means that the next player can move and capture the king)
			{
				print_message(WROND_BOARD_INITIALIZATION);
				flag = 1;
			}
			/*else if (!flag && getMoves(pieces, board, 1 - nextPlayer)->first == NULL) // mate or tie board
			{
				print_message(WROND_BOARD_INITIALIZATION);
				flag = 1;
			}*/
			if (!flag){ // can start the game
				return 0;
			}
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
	print_message(color);
	print_message(" player - enter your move: \n");
	FILE *fp = stdin;
	char input[51];
	int ch;
	while (1)
	{
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
			makeMove(m, pieces, board);
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
			piece *p = NULL;
			for (int i = 0; i < NUMOFPIECES; i++)
			{
				if (compPos(pieces[i].position, position) && pieces[i].captured == 0 && pieces[i].color == player)
				{
					p = pieces + i;
					break;
				}
			}
			if (p == NULL)
			{
				print_message(WRONG_PIECE);
				continue;
			}
			linkedList *moves = newLinkedList();
			genMoves(p, moves, board);
			printMoves(moves);
			freeList(moves);
		}
		else if (strncmp(input, "get_best_moves",14) == 0) // get best moves
		{
			linkedList *bestMoves = getBestMoves(board, pieces, player, input[15] - '0'); // !!! change this to include best difficulty !!!
			printMoves(bestMoves);
			freeList(bestMoves);
		}
		else if (strncmp(input, "get_score", 9) == 0) // get scores
		{
			int d = input[10] - '1';
			char* newBoard = (char*)malloc(120 * sizeof(char));
			memcpy(newBoard, board, 120 * sizeof(char));
			move *m = parseMove(input + 12,player);
			piece* newPieces = (piece*)malloc(NUMOFPIECES * sizeof(piece));
			memcpy(newPieces, pieces, NUMOFPIECES * sizeof(piece));
			makeMove(m, newPieces, newBoard);
			int *scores = NULL, score = alphabeta(newBoard, newPieces, player, INT_MIN, INT_MAX, depth, 1, 1, &scores);
			free(scores);
			free(newBoard);
			free(newPieces);
			free(m);
			printf("%d\n", score);
		}
		else if (strncmp(input, "quit", 4) == 0){
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
	linkedList *bestMoves = getBestMoves(board, pieces, 1 - userColor, depth);
	move *m = bestMoves->first->data;
	print_message("Computer: move ");
	printMove(m);
	makeMove(m, pieces, board);
	free(m);
	freeList(bestMoves);
	print_board(board);
	return endGamePrint(1 - userColor);
}

int consoleMode(){
	initPieces();
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