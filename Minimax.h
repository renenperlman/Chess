#ifndef MX_
#define MX_
#include "Engine.h"
#include <limits.h>

/*#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })*/

linkedList *getBestMoves(char*,int,int);
int alphabeta(char*, int , int , int , int , int , int **);
extern int pruneCount;

#endif 