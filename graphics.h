#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <stdbool.h>
#include <SDL.h>

// struct for in game buttons, integrated into the graphics struct as well
typedef struct gameButtons
{
	SDL_Rect pass;
	SDL_Rect save;
	SDL_Rect resign;
	SDL_Rect status;
	SDL_Texture* passTexture;
	SDL_Texture* saveTexture;
	SDL_Texture* resignTexture;
	SDL_Texture* invalidTexture;
	SDL_Texture* validTexture;
	SDL_Texture* statusTexture;
	SDL_Texture* continueTexture;
} gameButtons;

// struct for menu textures and the like
typedef struct menu
{
	SDL_Rect newG;
	SDL_Rect loadG;
	SDL_Rect exit;
	SDL_Texture* menu;
	SDL_Texture* newGTexture;
	SDL_Texture* loadGTexture;
	SDL_Texture* exitTexture;
} Menu;

// graphics struct
typedef struct graphics
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* background;
	SDL_Texture* backdrop;
	SDL_Texture* white;
	SDL_Texture* black;
	SDL_Texture* bWinner;
	SDL_Texture* wWinner;
	SDL_Texture* draw;
	gameButtons buttons;
	Menu menu;
	int HEIGHT;
	int WIDTH;
} Graphics;


bool initGameButtons(Graphics* window, gameButtons* buttons);
bool initMenu(Graphics* window);
bool startup(Graphics* window);

void drawBackdrop(Graphics* window);
void drawBackground(Graphics* window);
void drawContinue(Graphics* window);
void drawGameButtons(Graphics* window);
void drawStone(Graphics* window, int player, int x, int y);
void drawMenu(Graphics* window);
void drawWinner(Graphics* window, int winner);
void rerender();
void shutdown(Graphics* window);
void statusUpdate(Graphics* window, bool valid);
#endif
