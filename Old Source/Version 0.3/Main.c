#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define boardsize 19

// board struct
typedef struct
{
	unsigned int player;
	unsigned int liberties;
} Board;

// lovely constants
const int SCREEN_HEIGHT = 960;
const int SCREEN_WIDTH = 960;

// global variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* background = NULL;

Board board[boardsize][boardsize];

// prototypes
bool startup();
bool newgame();
bool shutdown();
int game();

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
	check = newgame();
	if (!check)
	{
		printf("Failed to create new game!\n");
		return 2;
	}

	// enter actual game function
	int winner = game();
	if (winner < 0)
	{
		printf("Error in Game! Error code: %i.", winner);
		return winner;
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


// game function
int game(void)
{
	// some lovely game logic variables
	bool quit = false;
	SDL_Event handler;
	int player = 1;
	int turncount = 0;
	int prisoners[2] = { 0, 0 };
	Board proposed[boardsize][boardsize];
	
	// stone texture initializations
	SDL_Texture* white = NULL;
	SDL_Texture* black = NULL;
	
	// create white stone texture
	SDL_Surface* holder = SDL_LoadBMP("white.bmp");
	white = SDL_CreateTextureFromSurface(renderer, holder);
	
	// create black stone texture
	holder = SDL_LoadBMP("black.bmp");
	black = SDL_CreateTextureFromSurface(renderer, holder);

	if (!black || !white)
	{
		printf("Failed to open stone textures!\n");
		return -1;
	}

	// surface freeing
	SDL_FreeSurface(holder);
	holder = NULL;


	// game loop
	while (!quit)
	{
		while (SDL_PollEvent(&handler))
		{
			switch (handler.type)
			{
			case SDL_QUIT:
				// checks if game window was exited
				quit = true;
				break;
			case SDL_MOUSEBUTTONUP:
				// checks for a left mouse click
				if (handler.button.button == SDL_BUTTON_LEFT)
				{
					int xcoord = handler.button.x;
					int ycoord = handler.button.y;
					printf("The x coordinate is: %i.\nThe y coordinate is: %i.\n", xcoord, ycoord);

					// checks if click is in bounds
					if ((xcoord > 955 || xcoord < 5) || (ycoord > 955 || ycoord < 5))
					{
						printf("Click out of bounds!\n");
						break;
					}

					// translate pixels to board coordinates
					int xbox = (xcoord - 5) / 50;
					int ybox = (ycoord - 5) / 50;


					
					// save previous board into proposed board for liberty calculations
					for (int i = 0; i < boardsize; i++)
					{
						for (int j = 0; j < boardsize; j++)
						{
							proposed[i][j].player = board[i][j].player;
						}
					}
					// change state of proposed board
					proposed[xbox][ybox].player = player;

					// TODO Calculate liberties of proposed move.
					proposed[xbox][ybox].liberties = 1; // placeholder for actual liberty logic


					// check validity of move and handle moar game logic
					if (board[xbox][ybox].player != 0 || proposed[xbox][ybox].liberties == 0)
					{
						printf("Move Invalid!\n");
					}
					else
					{
						// update space occupant
						board[xbox][ybox].player = player;

						// render new piece to board
						SDL_Rect move;
						move.x = 5 + (50 * xbox);
						move.y = 5 + (50 * ybox);
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
						
						// switches to other player
						if ((turncount % 2) == 0)
						{
							player++;
						}
						else
						{
							player--;
						}

						// turn count advances
						turncount++;
						printf("The tile row is: %i.\nThe tile column is: %i.\n", xbox, ybox);
					}
				}

				break;
			}

		}
	}

	// getting rid of textures
	SDL_DestroyTexture(white);
	SDL_DestroyTexture(black);

	// success!
	return 0;
}

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


	// success!
	return true;
}


bool newgame(void)
{
	// creating a surface to hold background image.
	SDL_Surface* screen = SDL_LoadBMP("background.bmp");
	if (!screen)
	{
		printf("Failed to initialize background! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// creates texture from surface
	SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, screen);
	if (!background)
	{
		printf("Failed to create texture from surface! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// free holder surface
	SDL_FreeSurface(screen);
	screen = NULL;
	
	// render background texture to window
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderPresent(renderer);

	// intialize the game board to 0 player values
	for (int i = 0; i < boardsize; i++)
	{
		for (int j = 0; j < boardsize; j++)
		{
			board[i][j].player = 0;
		}
	}

	// success!
	return true;
	
}


bool shutdown(void)
{
	// free used assets
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return true;
}