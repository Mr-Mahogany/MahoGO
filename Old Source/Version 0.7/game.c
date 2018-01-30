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
	game->prisoners[0] = 0;
	game->prisoners[1] = 0;
	int passcount = 0;
	Stone ko[boardsize][boardsize];
	int winner;


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
					
					// checks if click is within the board region
					if (xcoord < 960)
					{
						// checks if click is in bounds
						if ((xcoord > 955 || xcoord < 5) || (ycoord > 955 || ycoord < 5))
						{
							printf("Click out of bounds!\n");
							break;
						}

						// translate pixels to board coordinates
						int xbox = (xcoord - 5) / 50;
						int ybox = (ycoord - 5) / 50;

						Game proposed;

						// save previous board into proposed board for liberty calculations
						for (int i = 0; i < boardsize; i++)
						{
							for (int j = 0; j < boardsize; j++)
							{
								proposed.board[i][j].player = game->board[i][j].player;
								proposed.board[i][j].x = i;
								proposed.board[i][j].y = j;
								proposed.board[i][j].liberties = -1;
							}
						}
						// change state of proposed board
						proposed.board[xbox][ybox].player = player;

						// move validity check.
						bool valid = true;
						bool capture = false;
						bool suicide = false;
						bool copy = true;
						// space is already occupied, just go ahead and turn the valid flag off, no need to check liberties or ko violation
						if (game->board[xbox][ybox].player != 0)
						{
							valid = false;
						}
						else
						{
							// have to check liberties now
							for (int i = 0; i < boardsize; i++)
							{
								for (int j = 0; j < boardsize; j++)
								{
									bool check = calculateLiberties(&proposed, i, j);

									// if memory failed to allocate, we can't really do much else; terminate the game itself
									if (!check)
									{
										return -1;
									}
								}
							}


							// checks if move would self-capture (i.e. reduce one of the current player's stone's liberties to 0) and/or if an opponent's pieces are captured
							for (int i = 0; i < boardsize; i++)
							{
								for (int j = 0; j < boardsize; j++)
								{
									int opponent;
									if (player == 1)
									{
										opponent = 2;
									}
									else
									{
										opponent = 1;
									}

									// checks if an opponent piece was captured, if so the atari rule may apply, so we set the piece's player to 0 and prepare to recalculate liberties
									if (proposed.board[i][j].player == opponent && proposed.board[i][j].liberties == 0)
									{
										capture = true;
										proposed.board[i][j].player = 0;
										game->prisoners[player - 1]++;
									}

									if (proposed.board[i][j].player == player && proposed.board[i][j].liberties == 0 && !capture)
									{
										suicide = true;
									}

								}
							}
							// checks for ko rule violation; the Japanese Ko rule is used for its ease of implementation
							for (int i = 0; i < boardsize; i++)
							{
								for (int j = 0; j < boardsize; j++)
								{
									if (ko[i][j].player != proposed.board[i][j].player)
									{
										copy = false;
										i = boardsize;
										break;
									}
								}
							}

							// checks if suicide occurred and if it did it resulted in a capture; if a capture occurs, validity is still true due to atari rule
							// also checks for ko rule violation
							if ((suicide && !capture) || copy)
							{
								valid = false;
							}
						}

						// check validity of move and handle moar game logic
						if (!valid)
						{
							printf("Move Invalid!\n");
							statusUpdate(&game->window, false);
						}
						else
						{
							// set pass count to 0, passes are only added to see if both players passed for termination
							passcount = 0;

							// update space occupants and save the previous board state for ko checks
							for (int i = 0; i < boardsize; i++)
							{
								for (int j = 0; j < boardsize; j++)
								{
									ko[i][j].player = game->board[i][j].player;
									game->board[i][j].player = proposed.board[i][j].player;
								}
							}


							// checks if a capture occured in the move, rerendering the board, otherwise just drawing a new piece to the board
							if (capture)
							{
								rerender(game);
							}
							else
							{
								drawStone(&game->window, player, xbox, ybox);
							}

							// render message to update window
							statusUpdate(&game->window, true);

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
					else
					{
						// checks for a turn pass, adding othe passcount and passing accordingly
						if (xcoord >= game->window.buttons.pass.x && xcoord < game->window.buttons.pass.x + game->window.buttons.pass.w)
						{
							if (ycoord >= game->window.buttons.pass.y && ycoord < game->window.buttons.pass.y + game->window.buttons.pass.h)
							{
								passcount++;
								// switches to other player
								if ((turncount % 2) == 0)
								{
									player++;
								}
								else
								{
									player--;
								}
								turncount++;
							}
						}
						// checks if both players have passed, ending the game if so
						if (passcount == 2)
						{
							quit = true;
							break;
						}

						// checks if a player has resigned, ending the game if so and announcing the winner
						if (xcoord >= game->window.buttons.resign.x && xcoord < game->window.buttons.resign.x + game->window.buttons.resign.w)
						{
							if (ycoord >= game->window.buttons.resign.y && ycoord < game->window.buttons.resign.y + game->window.buttons.resign.h)
							{
								quit = true;
								if (player == 1)
								{
									winner = 2;
								}
								else
								{
									winner = 1;
								}
							}
						}


					}
				}

				break;
			}

		}
	}


	// success!
	return winner;
}

bool calculateLiberties(Game* group, int x, int y)
{
	// create a holder array for a potential string of stones
	Stone* holder = malloc((boardsize * boardsize) * sizeof(Stone));
	if (!holder)
	{
		printf("Failed to allocate memory for string holder!\n");
		return false;
	}

	// initialize all array player values to -1 to check if an element is occupied
	for (int i = 0; i < (boardsize * boardsize); i++)
	{
		holder[i].player = -1;
	}

	// counter for string's total liberties
	int liberties = 0;

	// makes sure if the player is not 0 for the provided starting coord, no need to waste time on unoccupied spaces
	if (group->board[x][y].player != 0)
	{
		// sets the first element of the array to the starting coord's data
		holder[0].player = group->board[x][y].player;
		holder[0].x = x;
		holder[0].y = y;

		
		int adder;
		int bound;
		int neighbors = 0;
		
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
					bound = 2;
				}
				else
				{
					bound = 0;
				}

				// checks the spaces vertically and horizontally adjacent to the current stone
				for (int j = adder; j < bound; j += 2)
				{
					// if space is empty, add to the liberties, if not check if it is occupied by the string player
					if (group->board[holder[i].x + j][holder[i].y].player == 0)
					{
						liberties++;
					}
					else if (group->board[holder[i].x + j][holder[i].y].player == holder[i].player)
					{
						// if the space is occupied the player, check if the neighboring stone is already in the string's array
						bool duplicate = false;
						int k = 0;
						while (holder[k].player != -1)
						{
							if ((holder[k].x == group->board[holder[i].x + j][holder[i].y].x) && (holder[k].y == group->board[holder[i].x + j][holder[i].y].y))
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
							holder[i + neighbors].player = holder[i].player;
							holder[i + neighbors].x = group->board[holder[i].x + j][holder[i].y].x;
							holder[i + neighbors].y = group->board[holder[i].x + j][holder[i].y].y;
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
						bound = 2;
					}
					else
					{
						bound = 0;
					}

					// checks the vertical neighbors of the current stone
					for (int j = adder; j < bound; j += 2)
					{
						// if adjacent vertical place is open, add liberty counter, if not check if it is occupied by the string's player
						if (group->board[holder[i].x][holder[i].y + j].player == 0)
						{
							liberties++;
						}
						else if (group->board[holder[i].x][holder[i].y + j].player == holder[i].player)
						{
							// check if place is already in the string array
							bool duplicate = false;
							int k = 0;
							while (holder[k].player != -1)
							{
								if ((holder[k].x == group->board[holder[i].x][holder[i].y + j].x) && (holder[k].y == group->board[holder[i].x][holder[i].y + j].y))
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
								holder[i + neighbors].player = holder[i].player;
								holder[i + neighbors].x = group->board[holder[i].x][holder[i].y + j].x;
								holder[i + neighbors].y = group->board[holder[i].x][holder[i].y + j].y;
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
			group->board[holder[i].x][holder[i].y].liberties = liberties;
			i++;
		}
		
		// free used heap memory
		free(holder);
		printf("%i\n", group->board[x][y].liberties);
		return true;
	}
	else
	{
		group->board[x][y].liberties = 0;
		// free used heap memory;
		free(holder);
		return true;
	}
	
	
}

bool newGame(Game* game)
{
	// render backdrop image
	drawBackdrop(&game->window);
	
	
	// render the background image.
	drawBackground(&game->window);
	
	// render side-menu buttons
	drawGameButtons(&game->window);

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


