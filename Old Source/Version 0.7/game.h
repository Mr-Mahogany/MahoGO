#ifndef GAME_H
#define GAME_H
#define boardsize 19

// board struct
typedef struct stone
{
	int player;
	int liberties;
	int x;
	int y;
} Stone;


// game struct
typedef struct game
{
	Graphics window;
	Stone board[boardsize][boardsize];
	int prisoners[2];
} Game;

int playGame();

bool newGame();

bool calculateLiberties();


#endif
