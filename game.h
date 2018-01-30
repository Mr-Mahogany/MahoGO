#ifndef GAME_H
#define GAME_H
#define boardsize 19
#include <SDL.h>
#include <stdbool.h>
#include "graphics.h"

// board struct
typedef struct stone
{
	int liberties;
	int player;
	int x;
	int y;
} Stone;

// game struct
typedef struct game
{
	bool exit;
	int prisoners[2];
	int turncount;
	Graphics window;
	Stone board[boardsize][boardsize];
	SDL_Event handler;
} Game;

bool calculateLiberties(Game* game, int x, int y);
bool saveGame(Game* game);

int newGame(Game* game);
int playGame(Game* game);
int scoreGame(Game* game);

void loadGame(Game* game);
void viewMenu(Game* game);





#endif
