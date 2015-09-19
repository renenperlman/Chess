#include "IO.h"

/*prints the given row to the given file*/
void saveRow(FILE *fp, char *board, int row){
	fprintf(fp, ROW, row);
	pos p;
	p.y = row;
	for (char i = 'a'; i <= 'h'; i++)
	{
		p.x = i;
		char c = board[posToInd(p)] == EMPTY ? '_' : board[posToInd(p)];
		fprintf(fp, "%c", c);
	}
	fprintf(fp, ROWEND, row);
}

void saveGame(FILE *fp, char *board){
	fprintf(fp, XMLHEAD);
	fprintf(fp, GAME);
	char* next = nextPlayer == WHITE ? "White" : "Black";
	fprintf(fp, NEXT, next);
	fprintf(fp, MODE, gameMode);
	if (gameMode == 2)
	{
		if (depth == 5){
			fprintf(fp,DIFFICULTYBEST);
		}
		else
		{
			fprintf(fp, DIFFICULTY, depth);
		}
		char* color = userColor == WHITE ? "White" : "Black";
		fprintf(fp, COLOR, color);
	}
	else
	{
		fprintf(fp, DIFFICULTYEMPTY);
		fprintf(fp, COLOREMPTY);
	}
	fprintf(fp, BOARD);
	for (int i = 8; i >= 1; i--){
		saveRow(fp, board, i);
	}
	fprintf(fp, BOARDEND);
	fprintf(fp, GAMEEND);
}

void loadRow(FILE *fp, int row, char* board, piece *pieces){
	pos p;
	p.y = row;
	char c;
	for (char i = 'a'; i <= 'h'; i++)
	{
		p.x = i;
		c = fgetc(fp);
		if (c!='_')
		{
			int color = islower(c) ? WHITE : BLACK;
			set(board, pieces,p, c, color);
		}
	}
}

void loadGame(FILE *fp, char *board, piece *pieces){
	int i = 0;
	char c;
	while (1){
		if (i < 2) // header and game lines
		{
		}
		if (gameMode == 1){
			if (i == 4 || i == 5)	//diffuclty and color lines
			{

			}
		}
		if (i == 6)	// board line
		{

		}
		if (i >= 15)
		{
			return;
		}
		while ((c = fgetc(fp)) != '>'){

		}
		if (i == 2) // next player line
		{
			nextPlayer = fgetc(fp) == 'W' ? WHITE : BLACK;
		}
		else if (i == 3){ // game mode line
			gameMode = fgetc(fp) - '0';
		}
		else if (i == 4) // difficulty line
		{				c = fgetc(fp);
			if (c == 'b')
			{					
				depth = 5;
			}
			else
			{					
				depth = c - '0';
			}
		}
		else if (i == 5) // user color line
		{
			userColor = fgetc(fp) == 'W' ? WHITE : BLACK;
		}
		if (i > 6) // row line
		{
			loadRow(fp, 8 + 7 - i, board, pieces);
		}
		
		while ((c = fgetc(fp)) != '\n' && c != EOF){

		}
		i++;
	}
}