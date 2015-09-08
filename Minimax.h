#include "DS.h"

int alphabeta(char*, piece*, int, int, int, int, int, int (score)(piece*, char*, int),
	linkedList* (getMoves)(piece*, char[], int), void (makeMove)(move*, piece*, char[]));