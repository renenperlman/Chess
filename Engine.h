#ifndef ENG_
#define ENG_


#include "DS.h"



void genMoves(pos, linkedList*, char[]); // adds to the list nodes containing all the possible moves by the given piece
linkedList* getMoves(char[], int); // returns a list containing all the possible moves by the given player
void updateMoveList(linkedList*, int, char[]); // assumes that the given player is in check,
//remove all moves that keeps in him check
void makeMove(move*, char[]);
int score(char[], int);
int isCheck(char[], int); // returns 1 if the given player is in check in the given board
int hasMoves(char*, int);
void promote(char*, pos, char); // promote the given pawn to the given type
int colorOfLoc(char*, int);
#endif ENG_
