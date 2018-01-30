#include "graphics.h"
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>



// graphics variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* background = NULL;
int SCREEN_HEIGHT = 960;
int SCREEN_WIDTH = 960;

// stone texture initializations
SDL_Texture* white = NULL;
SDL_Texture* black = NULL;

// startup SDL, create window, and renderer.
bool startup(void)
{
	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL failed to initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// create game window
	window = SDL_CreateWindow("MahoGO!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// create game renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("Renderer creation failed! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// initialize white and black stone textures
	SDL_Surface* holder = SDL_LoadBMP("white.bmp");
	white = SDL_CreateTextureFromSurface(renderer, holder);

	holder = SDL_LoadBMP("black.bmp");
	black = SDL_CreateTextureFromSurface(renderer, holder);

	if (!black || !white)
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
	
	background = SDL_CreateTextureFromSurface(renderer, screen);
	if (!background)
	{
		printf("Failed to create texture from surface! SDL_Error: %s\n", SDL_GetError());
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
bool shutdown(void)
{
	// free used assets
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	// getting rid of gametextures
	SDL_DestroyTexture(white);
	SDL_DestroyTexture(black);
	return true;
}

// render background onto window
bool drawBackground(void)
{

	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderPresent(renderer);

	return true;
}


// render new piece to board
void drawStone(int player, int x, int y)
{
	SDL_Rect move;
	move.x = 5 + (50 * x);
	move.y = 5 + (50 * y);
	move.w = 50;
	move.h = 50;
	if (player == 1)
	{
		SDL_RenderCopy(renderer, black, NULL, &move);
	}
	else
	{
		SDL_RenderCopy(renderer, white, NULL, &move);
	}
	
	SDL_RenderPresent(renderer);
}
