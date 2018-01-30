#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"
#include "game.h"

// defines the board variable
Stone board[boardsize][boardsize];


int playGame(void)
{
	// some lovely game logic variables
	bool quit = false;
	SDL_Event handler;
	int player = 1;
	int turncount = 0;
	int prisoners[2] = { 0, 0 };
	Stone proposed[boardsize][boardsize];

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
							proposed[i][j].x = i;
							proposed[i][j].y = j;
							proposed[i][j].liberties = -1;
						}
					}
					// change state of proposed board
					proposed[xbox][ybox].player = player;
					
					// move validity check.
					bool valid = true;

					if (board[xbox][ybox].player != 0)
					{
						valid = false;
					}
					else
					{
						// TODO Calculate liberties of board at large
						
						for (int i = 0; i < boardsize; i++)
						{
							for (int j = 0; j < boardsize; j++)
							{
								if (proposed[i][j].liberties < 0)
								{
									proposed[i][j].liberties = calculateLiberties(&proposed, i, j);
								}
							}
						}

						valid = true; // just a placeholder while I get the liberties sorted out so it doesn't auto reject moves.
					}

					// check validity of move and handle moar game logic
					if (!valid)
					{
						printf("Move Invalid!\n");
					}
					else
					{
						// update space occupant
						board[xbox][ybox].player = player;

						
						// render stone to board
						drawStone(player, xbox, ybox);

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


	// success!
	return 0;
}

int calculateLiberties(Stone* group, int x, int y)
{
	// insert liberty logic here
	int liberties = 1;
	return liberties;
}

bool newGame(void)
{
	// render the background image.
	bool check = drawBackground();
	if (!check)
	{
		printf("Failed to render background!\n");
		return false;
	}
	

	// intialize the game board to 0 player values and set the x and y values
	for (int i = 0; i < boardsize; i++)
	{
		for (int j = 0; j < boardsize; j++)
		{
			board[i][j].player = 0;
			board[i][j].x = i;
			board[i][j].y = j;
		}
	}

	// game start!
	int winner = playGame();
	if (winner < 0)
	{
		printf("Error in Game! Error code: %i.", winner);
		return false;
	}

	// success!
	return true;
}