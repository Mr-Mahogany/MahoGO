#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"
#include "game.h"

typedef struct game
{
	Graphics window;
	Stone board[boardsize][boardsize];

} Game;

int main(void)
{
	// startup SDL, game window, and whatnot
	bool check = startup();
	if (!check)
	{
		printf("Game startup failed!\n");
		return 1;
	}

	// start a new game (i.e. intialize a blank board)
	check = newGame();
	if (!check)
	{
		printf("Failed to create new game!\n");
		return 2;
	}


	
	check = shutdown();
	if (!check)
	{
		printf("IDK how, but shutdown failed!\n");
		return 9001;
	}

	// GG, success!
	return 0;
}










