#include "DS.h"

int WHITE_P_DIRECTIONS[] = { -10, -9, -11 };
int BLACK_P_DIRECTIONS[] = { 10, 9, 11 };
int N_DIRECTIONS[] = { -21, -19, -12, -8, 8, 12, 19, 21 };
int B_DIRECTIONS[] = { -11, -9, 9, 11 };
int R_DIRECTIONS[] = { -10, -1, 1, 10 };
int QnK_DIRECTIONS[] = { -11, -10, -9, -1, 1, 9, 10, 11 };

void genMoves(piece*, linkedList*, char[]); // adds to the list nodes containing all the possible moves by the given piece
linkedList* getMoves(piece*,char[], int); // returns a list containing all the possible moves by the given player
void updateMoveList(linkedList*,piece*,int,char[]); // assumes that the given player is in check,
													//remove all moves that keeps in him check
void makeMove(move*, piece*, char[]);
int score(piece[],char[],int);
int isCheck(piece[], char[], int); // returns 1 if the given player is in check in the given board
void promote(piece*, char); // promote the given pawn to the given type