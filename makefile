CFLAGS = -g -Wall `sdl-config--cflags`
SDL_FLAGS = $(shell sdl-config --cflags) -ISDL-ext
OBJECTS = chessprog.o Console.o GUI.o Best.o IO.o Minimax.o Engine.o Chess.o
HEADERS = Chess.h DS.h Engine.h Minimax.h IO.h Console.h Best.h GUI.h

chessprog : $(OBJECTS)
	gcc -o $@ $^ `sdl-config--libs` -std=c99

chessprog.o : chessprog.c $(HEADERS)
	gcc -c $*.c $(CFLAGS)

GUI.o : GUI2.c $(HEADERS)
	gcc -c $*.c $(CFLAGS)

Console.o : Console.c $(HEADERS)
	gcc -c $*.c $(CFLAGS)

Best.o : Best.c $(HEADERS)
	gcc -c $*.c $(CFLAGS)

IO.o : IO.c IO.h DS.h
	gcc -c $*.c $(CFLAGS)

Minimax.o : minimax.c $(HEADERS)
	gcc -c $*.c $(CFLAGS)

Engine.o : Engine.c $(HEADERS)
	gcc -c $*.c $(CFLAGS)

DS.o : DS.c $(HEADERS)
	gcc -c $*.c $(CFLAGS)

Chess.o : Chess.c $(HEADERS)
	gcc -c $*.c $(CFLAGS)

clean:
	-rm $(OBJECTS)
	-rm chessprog

