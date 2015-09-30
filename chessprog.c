#include "Console.h"
#include "GUI.h"

int main(char** argv, int argc){
	if (argc == 2 && strcmp(argv[1],"gui")==0) // run in gui mode
	{
		return GUImode();
	}
	return consoleMode(); // else run in console mode
}