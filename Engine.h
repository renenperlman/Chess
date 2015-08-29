#include "DS.h";

#define WHITE_P_DIRECTIONS {-10,-9,-11}
#define BLACK_P_DIRECTIONS {10,9,11}
#define K_DIRECTIONS {-21,-19,-12,-8,8,12,19,21}
#define B_DIRECTIONS {-11,-9,9,11}
#define R_DIRECTIONS {-10,-1,1,10}
#define QnK_DIRECTIONS {-11,-10,-9,-1,1,9,10,11}

void genMoves(piece*, linkedList*, char[]); // adds to the list nodes containing all the possible moves by the given piece
int score(piece[],int);
piece* newWhiteP(pos);
piece* newWhiteB(pos);
piece* newWhiten(pos);
piece* newWhiteR(pos);
piece* newWhiteQ(pos);
piece* newWhiteK(pos);
piece* newBlackP(pos);
piece* newBlackB(pos);
piece* newBlackN(pos);
piece* newBlackR(pos);
piece* newBlackK(pos);