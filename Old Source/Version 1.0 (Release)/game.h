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
	int turncount;
	bool exit;
	SDL_Event handler;
} Game;

int playGame();

int scoreGame();

int newGame();

bool calculateLiberties();

void viewMenu();

void loadGame();

bool saveGame();

#endif
