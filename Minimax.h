#ifndef MX_
#define MX_
#include "Engine.h"
#include <limits.h>

linkedList *getBestMoves(char*,int,int);
int alphabeta(char*, int , int , int , int , int , int **);


#endif 

#ifndef max
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif

#ifndef min
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif