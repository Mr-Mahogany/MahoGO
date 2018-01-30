#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <stdbool.h>
#include <SDL.h>
// graphics struct
typedef struct graphics
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* background;
	SDL_Texture* white;
	SDL_Texture* black;
	int HEIGHT;
	int WIDTH;
} Graphics;

bool startup();
bool shutdown();
bool drawBackground();
void drawStone();
#endif
