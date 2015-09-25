#ifndef ENG_
#define ENG_


#include "DS.h"



void genMoves(piece*, linkedList*, char[]); // adds to the list nodes containing all the possible moves by the given piece
linkedList* getMoves(piece*, char[], int,int); // returns a list containing all the possible moves by the given player
void updateMoveList(linkedList*, piece*, int, char[]); // assumes that the given player is in check,
//remove all moves that keeps in him check
void makeMove(move*, piece*, char[]);
int score(piece[], char[], int);
int isCheck(piece[], char[], int); // returns 1 if the given player is in check in the given board
int hasMoves(piece*, char*, int);
void promote(piece*, char); // promote the given pawn to the given type
#endif ENG_
