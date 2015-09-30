#ifndef ENG_
#define ENG_


#include "DS.h"



void genMoves(pos, linkedList*, char[]); // adds to the list nodes containing all the possible moves by the given piece
linkedList* getMoves(char[], int); // returns a list containing all the possible moves by the given player
void makeMove(move*, char[]); 
void unmakeMove(move*, char*,char); // revert the given move
int score(char[], int); // compute the score of the board
int isCheck(char[], int); // returns 1 if the given player is in check in the given board
int hasMoves(char*, int); // returns 1 if the given player has possible moves
void promote(char*, pos, char); // promote the given pawn to the given type
int colorOfLoc(char*, int);


#endif 
