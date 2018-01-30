#include "graphics.h"
#include "game.h"
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>




// startup SDL, create window, and renderer.
bool startup(Graphics* window)
{
	window->HEIGHT = 960;
	window->WIDTH = 1180;
	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL failed to initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// create game window
	window->window = SDL_CreateWindow("MahoGO!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window->WIDTH, window->HEIGHT, SDL_WINDOW_SHOWN);
	if (!window->window)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// create game renderer for window
	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
	if (!window->renderer)
	{
		printf("Renderer creation failed! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// initialize white and black stone textures
	SDL_Surface* holder = SDL_LoadBMP("white.bmp");
	window->white = SDL_CreateTextureFromSurface(window->renderer, holder);

	holder = SDL_LoadBMP("black.bmp");
	window->black = SDL_CreateTextureFromSurface(window->renderer, holder);

	if (!window->black || !window->white)
	{
		printf("Failed to open stone textures!\n");
		return false;
	}

	// initialize background
	SDL_Surface* screen = SDL_LoadBMP("background.bmp");
	if (!screen)
	{
		printf("Failed to initialize background! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	window->background = SDL_CreateTextureFromSurface(window->renderer, screen);
	if (!window->background)
	{
		printf("Failed to create texture from surface! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// create color backdrop texture
	screen = SDL_LoadBMP("color.bmp");
	if (!screen)
	{
		printf("Failed to initialize monocolor backdrop!\n");
		return false;
	}
	window->backdrop = SDL_CreateTextureFromSurface(window->renderer, screen);
	if (!window->backdrop)
	{
		printf("Failed to create color texture!\n");
		return false;
	}

	// initialize button textures and button rect locations
	bool check = initGameButtons(window, &window->buttons);
	if (!check)
	{
		printf("Failed to initialize game buttons!\n");
		return false;
	}

	check = initMenu(window);
	if (!check)
	{
		printf("Failed to initialize menu!\n");
		return false;
	}

	// surface freeing
	SDL_FreeSurface(holder);
	SDL_FreeSurface(screen);
	holder = NULL;
	screen = NULL;

	// success!
	return true;
}

// SDL shutdown and app cleanup
bool shutdown(Graphics* window)
{
	// free used assets
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);

	// getting rid of gametextures
	SDL_DestroyTexture(window->white);
	SDL_DestroyTexture(window->black);
	SDL_DestroyTexture(window->background);
	SDL_DestroyTexture(window->backdrop);
	SDL_DestroyTexture(window->buttons.passTexture);
	SDL_DestroyTexture(window->buttons.resignTexture);
	SDL_DestroyTexture(window->buttons.saveTexture);
	SDL_DestroyTexture(window->buttons.validTexture);
	SDL_DestroyTexture(window->buttons.invalidTexture);
	SDL_DestroyTexture(window->buttons.statusTexture);

	SDL_Quit();
	
	
	return true;
}

// render color backdrop
void drawBackdrop(Graphics* window)
{
	SDL_RenderClear(window->renderer);
	SDL_RenderCopy(window->renderer, window->backdrop, NULL, NULL);
	SDL_RenderPresent(window->renderer);
	return;
}

// render board background onto window
void drawBackground(Graphics* window)
{
	SDL_Rect gameSpace;
	gameSpace.x = 0;
	gameSpace.y = 0;
	gameSpace.w = 960;
	gameSpace.h = 960;

	SDL_RenderCopy(window->renderer, window->background, NULL, &gameSpace);
	SDL_RenderPresent(window->renderer);

	return;
}

// render new piece to board
void drawStone(Graphics* window, int player, int x, int y)
{
	SDL_Rect move;
	move.x = 5 + (50 * x);
	move.y = 5 + (50 * y);
	move.w = 50;
	move.h = 50;
	if (player == 1)
	{
		SDL_RenderCopy(window->renderer, window->black, NULL, &move);
	}
	else
	{
		SDL_RenderCopy(window->renderer, window->white, NULL, &move);
	}
	
	SDL_RenderPresent(window->renderer);
}

// rerender entire board, primarily used in the event of piece capture
void rerender(Game* game)
{
	SDL_RenderClear(game->window.renderer);

	drawBackdrop(&game->window);

	drawBackground(&game->window);

	drawGameButtons(&game->window);

	for (int i = 0; i < boardsize; i++)
	{
		for (int j = 0; j < boardsize; j++)
		{
			if (game->board[i][j].player == 1)
			{
				drawStone(&game->window, 1, i, j);
			}
			else if (game->board[i][j].player == 2)
			{
				drawStone(&game->window, 2, i, j);
			}
		}
	}

	SDL_RenderPresent(game->window.renderer);
}

// intialize game button positions and their respective textures
bool initGameButtons(Graphics* window, gameButtons* buttons)
{
	// setting game button window positions for later renders and click checks
	buttons->pass.x = 965;
	buttons->pass.y = 96;
	buttons->pass.w = 180;
	buttons->pass.h = 120;

	buttons->resign.x = 965;
	buttons->resign.y = 312;
	buttons->resign.w = 180;
	buttons->resign.h = 120;

	buttons->save.x = 965;
	buttons->save.y = 528;
	buttons->save.w = 180;
	buttons->save.h = 120;

	buttons->status.x = 965;
	buttons->status.y = 744;
	buttons->status.w = 180;
	buttons->status.h = 120;

	// initializing all game button textures checking if they successfully loaded
	SDL_Surface* holder = SDL_LoadBMP("pass.bmp");
	if (!holder)
	{
		printf("Failed to load pass.bmp\n");
		return false;
	}
	buttons->passTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!buttons->passTexture)
	{
		printf("Failed to create pass texture!\n");
		return false;
	}


	holder = SDL_LoadBMP("resign.bmp");
	if (!holder)
	{
		printf("Failed to load resign.bmp\n");
		return false;
	}
	buttons->resignTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!buttons->resignTexture)
	{
		printf("Failed to create resign texture!\n");
		return false;
	}

	holder = SDL_LoadBMP("save.bmp");
	if (!holder)
	{
		printf("Failed to load save.bmp\n");
		return false;
	}
	buttons->saveTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!buttons->saveTexture)
	{
		printf("Failed to create save texture!\n");
		return false;
	}

	holder = SDL_LoadBMP("blank.bmp");
	if (!holder)
	{
		printf("Failed to load blank.bmp\n");
		return false;
	}
	buttons->statusTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!buttons->statusTexture)
	{
		printf("Failed to create status texture!\n");
		return false;
	}

	holder = SDL_LoadBMP("valid.bmp");
	if (!holder)
	{
		printf("Failed to load valid.bmp\n");
		return false;
	}
	buttons->validTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!buttons->validTexture)
	{
		printf("Failed to create valid texture!\n");
		return false;
	}

	holder = SDL_LoadBMP("invalid.bmp");
	if (!holder)
	{
		printf("Failed to load invalid.bmp\n");
		return false;
	}
	buttons->invalidTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!buttons->invalidTexture)
	{
		printf("Failed to create invalid texture!\n");
		return false;
	}

	holder = SDL_LoadBMP("continue.bmp");
	if (!holder)
	{
		printf("Failed to load continue.bmp\n");
		return false;
	}
	buttons->continueTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!buttons->continueTexture)
	{
		printf("Failed to create continue texture!\n");
		return false;
	}

	SDL_FreeSurface(holder);
	// success!
	return true;
}

// initialize textures and values for game menu
bool initMenu(Graphics* window)
{
	// menu button rectangle value initialization
	window->menu.newG.x = 430;
	window->menu.newG.w = 320;
	window->menu.newG.y = 330;
	window->menu.newG.h = 140;

	window->menu.loadG.x = 430;
	window->menu.loadG.w = 320;
	window->menu.loadG.y = 540;
	window->menu.loadG.h = 140;

	window->menu.exit.x = 430;
	window->menu.exit.w = 320;
	window->menu.exit.y = 750;
	window->menu.exit.h = 140;

	// create various button and menu background textures
	SDL_Surface* holder = SDL_LoadBMP("newgame.bmp");
	if (!holder)
	{
		printf("Failed to load newgame.bmp!\n");
		return false;
	}
	window->menu.newGTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!window->menu.newGTexture)
	{
		printf("Failed to create new game button texture!\n");
		return false;
	}

	holder = SDL_LoadBMP("loadgame.bmp");
	if (!holder)
	{
		printf("Failed to load loadgame.bmp!\n");
		return false;
	}
	window->menu.loadGTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!window->menu.loadGTexture)
	{
		printf("Failed to create load game button texture!\n");
		return false;
	}

	holder = SDL_LoadBMP("exit.bmp");
	if (!holder)
	{
		printf("Failed to load exit.bmp!\n");
		return false;
	}
	window->menu.exitTexture = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!window->menu.exitTexture)
	{
		printf("Failed to create exit game button texture!\n");
		return false;
	}

	holder = SDL_LoadBMP("menu.bmp");
	if (!holder)
	{
		printf("Failed to load menu.bmp!\n");
		return false;
	}
	window->menu.menu = SDL_CreateTextureFromSurface(window->renderer, holder);
	if (!window->menu.menu)
	{
		printf("Failed to create menu texture!\n");
		return false;
	}

	// success!
	return true;

}

void drawMenu(Graphics* window)
{
	SDL_RenderClear(window->renderer);

	SDL_RenderCopy(window->renderer, window->menu.menu, NULL, NULL);
	SDL_RenderCopy(window->renderer, window->menu.newGTexture, NULL, &window->menu.newG);
	SDL_RenderCopy(window->renderer, window->menu.loadGTexture, NULL, &window->menu.loadG);
	SDL_RenderCopy(window->renderer, window->menu.exitTexture, NULL, &window->menu.exit);

	SDL_RenderPresent(window->renderer);

}

// render game buttons to screen
void drawGameButtons(Graphics* window)
{
	SDL_RenderCopy(window->renderer, window->buttons.passTexture, NULL, &window->buttons.pass);
	SDL_RenderCopy(window->renderer, window->buttons.resignTexture, NULL, &window->buttons.resign);
	SDL_RenderCopy(window->renderer, window->buttons.saveTexture, NULL, &window->buttons.save);
	SDL_RenderCopy(window->renderer, window->buttons.statusTexture, NULL, &window->buttons.status);

	SDL_RenderPresent(window->renderer);
}

// render the continue button
void drawContinue(Graphics* window)
{
	SDL_RenderCopy(window->renderer, window->buttons.continueTexture, NULL, &window->buttons.save);

	SDL_RenderPresent(window->renderer);
}
// render a status message
void statusUpdate(Graphics* window, bool valid)
{
	if (!valid)
	{
		SDL_RenderCopy(window->renderer, window->buttons.invalidTexture, NULL, &window->buttons.status);
		SDL_RenderPresent(window->renderer);
		SDL_Delay(750);
		
		SDL_RenderCopy(window->renderer, window->buttons.statusTexture, NULL, &window->buttons.status);
		SDL_RenderPresent(window->renderer);
	}
	else
	{
		SDL_RenderCopy(window->renderer, window->buttons.validTexture, NULL, &window->buttons.status);
		SDL_RenderPresent(window->renderer);
		SDL_Delay(750);

		SDL_RenderCopy(window->renderer, window->buttons.statusTexture, NULL, &window->buttons.status);
		SDL_RenderPresent(window->renderer);
	}
}