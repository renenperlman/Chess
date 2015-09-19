#ifndef IO_
#define IO_

#include "DS.h"

#define XMLHEAD "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define GAME "<game>\n"
#define NEXT "\t<next_turn>%s</next_turn>\n"
#define MODE "\t<game_mode>%d< / game_ mode >\n"
#define DIFFICULTY "\t<difficulty>%d< / difficulty>\n"
#define DIFFICULTYBEST "\t<difficulty>best< / difficulty>\n"
#define DIFFICULTYEMPTY "\t<difficulty>< / difficulty>\n"
#define COLOR "\t<user_color>%s< / user_color>\n"
#define COLOREMPTY "\t<user_color>< / user_color>\n"
#define BOARD "\t<board>\n"
#define ROW "\t\t<row_%d>"
#define ROWEND "</row_%d>\n"
#define BOARDEND "\t</board>\n"
#define GAMEEND "</game>"



/*save the given board to the given file*/
void saveGame(FILE*, char*);

/*loads the given file to the given board*/
void loadGame(FILE*, char*, piece*);

#endif IO_