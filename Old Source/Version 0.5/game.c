#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"
#include "game.h"

int playGame(Game* game)
{
	// some lovely game logic variables
	bool quit = false;
	SDL_Event handler;
	int player = 1;
	int turncount = 0;
	int prisoners[2] = { 0, 0 };
	Stone proposed[boardsize][boardsize];

	corruption(game);
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
							proposed[i][j].player = game->board[i][j].player;
							proposed[i][j].x = i;
							proposed[i][j].y = j;
							proposed[i][j].liberties = -1;
						}
					}
					// change state of proposed board
					proposed[xbox][ybox].player = player;
					
					// move validity check.
					bool valid = true;

					if (game->board[xbox][ybox].player != 0)
					{
						valid = false;
					}
					else
					{
						
						for (int i = 0; i < boardsize; i++)
						{
							for (int j = 0; j < boardsize; j++)
							{
								if (proposed[i][j].liberties < 0)
								{
									calculateLiberties(proposed, i, j, player);
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
						game->board[xbox][ybox].player = player;

						
						// render stone to board
						drawStone(&game->window, player, xbox, ybox);

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

void calculateLiberties(Stone** group, int x, int y, int player)
{
	// create a holder array for a potential string of stones
	Stone* holder = malloc((boardsize * boardsize) * sizeof(Stone));

	// initialize all array player values to -1 to check if an element is occupied
	for (int i = 0; i < (boardsize * boardsize); i++)
	{
		holder[i].player = -1;
	}

	// counter for string's total liberties
	int liberties = 0;

	// makes sure if the player is not 0 for the provided starting coord, no need to waste time on unoccupied spaces
	if (group[x][y].player != 0)
	{
		// sets the first element of the array to the starting coord's data
		holder[0].player = group[x][y].player;
		holder[0].x = x;
		holder[0].y = y;

		
		int adder;
		int bound;
		int neighbors;
		
		// begins calculating the liberties of individual spaces and their adjacency to stones
		for (int i = 0; i < (boardsize * boardsize); i++)
		{
			// checks if the holder element is actually part of a string
			if (holder[i].player != -1)
			{
				// checks if the x coord is at the left boundary point
				if (holder[i].x > 0)
				{
					adder = -1;
				}
				else
				{
					adder = 1;
				}
				
				// checks if the x coord is at the right boundary point
				if (holder[i].x < (boardsize - 1))
				{
					adder = -1;
				}
				else
				{
					bound = 0;
				}

				// checks the spaces vertically and horizontally adjacent to the current stone
				for (int j = adder; j < bound; j += 2)
				{
					// if space is empty, add to the liberties, if not check if it is occupied by the string player
					if (group[holder[i].x + j][holder[i].y].player == 0)
					{
						liberties++;
					}
					else if (group[holder[i].x + j][holder[i].y].player == holder[i].player)
					{
						// if the space is occupied the player, check if the neighboring stone is already in the string's array
						bool duplicate = false;
						int k = 0;
						while (holder[k].player != -1)
						{
							if ((holder[k].x == group[holder[i].x + j][holder[i].y].x) && (holder[k].y == group[holder[i].x + j][holder[i].y].y))
							{
								duplicate = true;
								break;
							}

							k++;
						}

						// if this stone is new, add it to the array
						if (!duplicate)
						{
							neighbors++;
							holder[i + neighbors].player = player;
							holder[i + neighbors].x = group[holder[i].x + j][holder[i].y].x;
							holder[i + neighbors].y = group[holder[i].x + j][holder[i].y].y;
						}
					}

					// checks if the y coord is at the top boundary point
					if (holder[i].y > 0)
					{
						adder = -1;
					}
					else
					{
						adder = 1;
					}

					// checks if the y coord is at the bottom boundary point
					if (holder[i].y < (boardsize - 1))
					{
						adder = -1;
					}
					else
					{
						bound = 0;
					}

					// checks the vertical neighbors of the current stone
					for (int j = adder; j < bound; j += 2)
					{
						// if adjacent vertical place is open, add liberty counter, if not check if it is occupied by the string's player
						if (group[holder[i].x][holder[i].y + j].player == 0)
						{
							liberties++;
						}
						else if (group[holder[i].x][holder[i].y + j].player == holder[i].player)
						{
							// check if place is already in the string array
							bool duplicate = false;
							int k = 0;
							while (holder[k].player != -1)
							{
								if ((holder[k].x == group[holder[i].x][holder[i].y + j].x) && (holder[k].y == group[holder[i].x][holder[i].y + j].y))
								{
									duplicate = true;
									break;
								}

								k++;
							}

							// if not a duplicate add this new stone to the string
							if (!duplicate)
							{
								neighbors++;
								holder[i + neighbors].player = player;
								holder[i + neighbors].x = group[holder[i].x][holder[i].y + j].x;
								holder[i + neighbors].y = group[holder[i].x][holder[i].y + j].y;
							}
						}
					}
				}

			}
			else
			{
				// no more stones in the string!
				break;
			}
			// since the next stone is being considered, the neighbor count decreased by one assuming it is greater than 0
			if (neighbors > 0)
			{
				neighbors--;
			}
		}
		// applies the liberty counter to all stones in the string of stones
		int i = 0;
		while (holder[i].player != -1)
		{
			group[holder[i].x][holder[i].y].liberties = liberties;
		}
		
		// free used heap memory
		free(holder);
		return;
	}
	else
	{
		group[x][y].liberties = 0;
		// free used heap memory;
		return;
	}
	
	
}

bool newGame(Game* game)
{
	// render the background image.
	bool check = drawBackground(&game->window);
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
			game->board[i][j].player = 0;
			game->board[i][j].x = i;
			game->board[i][j].y = j;
		}
	}

	// game start!
	int winner = playGame(game);
	if (winner < 0)
	{
		printf("Error in Game! Error code: %i.", winner);
		return false;
	}

	// success!
	return true;
}

