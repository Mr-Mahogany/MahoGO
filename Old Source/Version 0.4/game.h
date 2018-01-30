#ifndef GAME_H
#define GAME_H
#define boardsize 19

// board struct
typedef struct stone
{
	unsigned int player;
	int liberties;
	int x;
	int y;
} Stone;

int playGame();

bool newGame();

int calculateLiberties();

#endif
