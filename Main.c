#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"
#include "game.h"



int main(void)
{

	Game game;
	// startup SDL, game window, and whatnot
	bool check = startup(&game.window);
	if (!check)
	{
		printf("Game startup failed!\n");
		return 1;
	}

	// enter into game menu
	viewMenu(&game);

	// shutdown and free various assets
	shutdown(&game.window);
	
	// GG, success!
	return 0;
}










