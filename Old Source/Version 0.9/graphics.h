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
	gameButtons buttons;
	Menu menu;
	int HEIGHT;
	int WIDTH;
} Graphics;



bool startup();
bool shutdown();
bool initGameButtons();
bool initMenu();
void drawStone();
void rerender();
void drawBackground();
void drawBackdrop();
void drawGameButtons();
void statusUpdate();
void drawContinue();
void drawMenu();
#endif
