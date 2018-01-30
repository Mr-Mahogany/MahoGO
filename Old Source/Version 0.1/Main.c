
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(void)
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen = NULL;

	SDL_Surface* test = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL failed to initialize. SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	//Create window
	window = SDL_CreateWindow("Calibration Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 2;
	}

	screen = SDL_GetWindowSurface(window);
	test = SDL_LoadBMP("test.bmp");
	if (!test)
	{
		printf("Failed to load image!");
		return 3;
	}

	SDL_BlitSurface(test, NULL, screen, NULL);
	SDL_UpdateWindowSurface(window);

	SDL_FreeSurface(test);
	test = NULL;
	bool quit = false;
	SDL_Event handler;

	while (!quit)
	{
		while (SDL_PollEvent(&handler) != 0)
		{
			//User requests quit
			if (handler.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}


	SDL_FreeSurface(screen);

	SDL_Quit();

	return 0;
}