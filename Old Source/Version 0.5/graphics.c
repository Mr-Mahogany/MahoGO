#include "graphics.h"
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>




// startup SDL, create window, and renderer.
bool startup(Graphics* window)
{
	window->HEIGHT = 960;
	window->WIDTH = 960;
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
	SDL_Quit();
	
	// getting rid of gametextures
	SDL_DestroyTexture(window->white);
	SDL_DestroyTexture(window->black);
	return true;
}

// render background onto window
bool drawBackground(Graphics* window)
{

	SDL_RenderCopy(window->renderer, window->background, NULL, NULL);
	SDL_RenderPresent(window->renderer);

	return true;
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
