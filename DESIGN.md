I) Introduction
	MahoGO is a Windows desktop application built from the ground up (excluding libraries of course) in C. The application is made to allow two players at the same PC to play the board
game Go. The application is built using the SDL2 library (windows version (I'm not certain if there is an actual difference between window's versions of the libraries, but I digress))
and was built in Visual Studio and thus tested using the VC++ compiler. The game remains untested in other compilers or on an OS other than Windows 10, however, it should still operate
(I suspect) on previous versions of Windows or even potentially (with low confidence) in WINE environments. Since there are several rulesets of Go around the world, for design purposes,
1 version of each conflicting rule was chosen arbitrarily. I chose to implement the Japanese variations of these rules (particularly in regards to the Ko rule and scoring).

	There are 3 source files in total and two header files. The 3 source files are Main.c, game.c, and graphics.c. The 2 header files are game.h and graphics.h. As a whole, I tried to 
approach the problem with a modular design approach and a relatively high degree of decoupling for organization purposes, although the organization of the code itself dropped as the 
deadline began to approach (sorry for that in advance). As a disclosure, a friend from back home helped me debug my code and made various organization suggestions (such as the 
creation of the Graphics and Game structs) throughout the process of making this; however, it should be noted that all the code itself is written myself and the only algorithms I took 
any external inspiration for were the calculate liberties and territory scoring algorithms which were inspired from the four-way flood fill algorithm. I also decided to take manual 
input for dead stones/strings after reading a reddit post that suggested doing so programatically would be extremely difficult or verbatim "distressingly non-trivial", but I digress. 
The project as a whole has been extremely enjoyable and although I wish I could have implemented more features or the current version for flexibly, I think that the current application
reached a reasonable portion of my goals, and I may very well revisit this project later to implement some of the other features I had hoped to.

II) The Main Function
	I am starting the source code walkthrough with Main.c. This is by far the shortest of the source code, only containing the main function which acts exclusively as a sort of handler
function, linking the other parts of the code together. It begins by declaring a Game struct (game) that contains all major variables that are used across multiple functions. The game
struct contains a Graphics struct (another custom struct), a Stone 2d array of size 19x19 meant to represent the board, and exit boolean, the prisoner counters, and the turn count. Each
of these variables is used in multiple functions and passed to them by way of pointers to this original struct. After the game struct's declaration, main calls the startup function, a
function in graphics.c which initializes various graphical assets, and checks if the initialization was successful by way of the returned boolean, if startup failed, it exits the game
with error code 1. After startup, it then proceeds to the viewMenu function which is the menu loop (more on that later) and after the menu loop exits, it calls the shutdown function
which frees all the used assets from the heap as to plug up any potential memory leaks.

III) The Graphics Functions
	The first graphics function called is the startup() which initializes various game assets from their respective files. It begins by starting up all modules of SDL and creating
a window. Startup is passed the game struct's Graphics struct as a pointer to set it's values for later usage. Startup creates a window and a renderer so other graphics functions can
actually display info, it also intializes all textures in the game. This is done either directly in the function, in the case of the backdrop, board texture, stone textures, and a couple
others or by calling an additional initialization function. Additional initialization functions were used for large groups of textures that were highly related to one another such as the
in-game buttons and the menu textures. 

	Most other graphical functions are used to draw various textures onto the screen in their respective locations; drawBackdrop draws a monocolor backdrop (matches the board's color) 
so the in-game sidebar has a color, drawBackground draws the game board texture, drawContinue replaces the save button with the continue button in the dead stone selection phase of the 
game (more on that later) drawGameButtons draws the various in-game buttons on their sidebar (e.g. the pass and resign buttons), drawMenu renders the menu background and menu buttons
in the menu loop as well as clearing the render beforehand, drawStone draws a new stone of the specified player in the specified space on the board, drawWinner renders a winner
announcement box at the end of a game, rerender rerenders the entire game board (primarily used in the event of captures), and statusUpdate provides the invalid or valid move popup 
messages in the status box toward the bottom right of the game screen. The last graphical function is shutdown which just frees up used graphical resources from the heap to prevent
memory leaks.

IV) The Game Functions
	The first game function that should be seen by any player (and the one directly called by the main function) is the viewMenu function. This function takes a pointer to the game
struct as a parameter (as most other functions do in some capacity). This function begins by rendering the menu itself by calling drawMenu; this renders the menu background and buttons.
ViewMenu is mostly just an event handler system; it checks for window exits or for a user clicking on a menu button. Upon clicking on a menu button, it will call the appropriate function
newGame() for the New Game button and loadGame() for the Load Game button. If "Exit MahoGO" is selected, it exits the application itself. 
	
	newGame() is used to generate a new game. It begins by calling drawBackdrop and drawBackground, and drawGameButtons, rendering the game environment, it then intializes the game
turncount and and player prisoner counts to 0 as to reflect a brand new game. All other game logic variables are declared in playGame and are derivative from these that are present
in the game struct. loadGame() attempts to open "save.mah" file for reading. "save.mah" is a file created by the game upon the first successful save, it stores the current turncount,
respective prisoner counters, and the board state. If "save.mah" doesn't exist or if the fopen call fails in some other capacity, rather than returning an error and doing nothing, the
function calls newGame() so as to not delay the player from entering the game. Upon a save in game, "save.mah" is created if necessary or truncated to 0 before a rewrite. This ensures
the save file doesn't bloat over time. There can only be one saved game at a time, this was chosen due to time restrictions, but if I revisit the project, I plan to implement
multiple save files.

	Upon calling newGame or loadGame, both of which initialize the game struct's value, the functions then call the playGame() function. playGame() contains the largest bulk of the 
game logic including the primary game loop itself. playGame begins by initializing a few game logic variables that are not part of the game struct itself (as they are derived from
game struct variables). Among these variables are the current player - symbolized by an integer - a quit boolean, a resign boolean and a 2d Stone array which is used to save the
previous state of the board to check for Ko rule violations. The player is determined from the current turncount. After the variable declarations and intializations, the main game loop
begins. The game loop goes into an SDL event handler loop which checks for mouse clicks and window exits. If a mouse click is detected, it is checked if it was the left mouse button,
proceeding with logic if it was. The event only occurs upon release of the mouse button (SDL_MOUSEBUTTONUP) to allow players to click and hold their mouse to potenially select other
spaces if they misclick or dislike their initial choice. If a left click is detected, it then gets the mouse's x and y coordinates (in pixels) and checks if they are in the board
region (x pixel < 960). If they are in the region, it then converts the x and y coordinates to their respective space coordinate to be used by a 2d stone array (variables that 
represent game boards).

	Upon selecting a valid space, a proposed game struct is created for validity checks. If the space is already occupied, the move is invalid and the player must select again,
otherwise, the proposed board is passed into calculateLiberties to check the move's validity. calculateLiberties determines the liberties for every stone on the board. The function
is passed a game struct (to ease the pain of pointer parameters) which it then uses to manipulate the stone liberty variables directly. It is also passed a starting x and y coordinate.
The function begins by checking if the liberties for that stone have already been calculated, uncalculated liberties are set to -2 for easy recognition. It then starts at the starting
coordinates and checks the coordinates vertical and horizontal neighbors, making sure to be careful of being at the boundaries of the board. If a neighboring space is empty and function
wide liberty counter is added to; if it is occupied by the same player's stone, it is then added to a holder array (an array of size 19 * 19) after a check to make sure it is not 
already on the array. This process is repeated for every member of the holder array - an array's elements are considered empty if the player variable is equal to -1 (once again chosen
for ease of recognition). Once the holder array is fully accounted for, the liberty counter is applied to every member of the holder array (or rather their proposed board counterparts).
This is repeated for every stone on the board. After a full board calculation, the game function checks the liberties of every stone, if the proposed board results in an enemy capture,
(an opponent's stone has its liberties reduced to 0), the move is accepted regardless regardless of reducing one's own liberties to 0 (atari rule); if a capture does not occur, but 
a player reduces their own liberties to 0 for a stone, the move is rejected and the player is requested to move again (suicide rule). If a capture occurs (assuming the move was
valid), the opponent's captured spaces are set to empty (player = 0), the player's prisoner count is added to, and the entire board is rerendered (via rerender()).

	After checking the liberties, a quick Ko rule check occurs. The Ko rule flag is set default to true and each element's player value of the ko board and the proposed board are 
compared against one another, if even one difference is found, the Ko flag is set to false, and thus the move is valid; this is an interpretation of the Japanese Ko rule which only
takes the immediate last board state into consideration, but not ALL previous board states; this was done to ease the implementation, create consistency with other rules, and save
space and speed. If a move is valid, it then sets the actual board state to reflect that, draws the stone (drawStone) or rerenders the board in case of capture.

	If the x coordinate of the click is outside the game board (x pixel > 960), the game then checks if the click was on one of the buttons. If the click was on pass, the passcounter
is added to (it is reset if a valid move occurs) and the turncounter advances. If resign was clicked, the game loop is broken out of, scoring does not occur and the opponent is 
announced as the winner. Finally, Save Game calls the saveGame() function which saves the current game state to "save.mah".
	

	This process of checks, acceptance, and rejection is continued until the primary game loop is terminated. The game is terminated in one of three ways. The first way is when both
players pass consecutively; this results in dead stone removal phase (more on that in a bit) and scoring. The second is one player resigns; no scoring or stone removal occurs and the
opponent is automatically announced as the winner. The third is just by exiting the window itself; no announcement occurs and neither does scoring. 

	After quiting the main game loop, a second loop occurs if the game loop ended by passes. This game loop is used for the players to select any dead stones or strings. A dead stone
is one that is inside enemy territory but cannot survive on its own. There is no easy way to determine this programatically, so the players select the stones manually. After all stones
are selected, they may press continue (now taking up the save button on right hand bar) which then proceeds to the scoring algorithm. If the players fail to recognize a dead stone,
the scoring algorithm will not work as accurately as possible.

	The scoring algorithm works very similarly to the calculateLiberties algorithm, only this time focusing on empty spaces. It checks empty spaces for neighboring empty spaces, adding
them to a holder array if found and flagging them (by way of the liberty counter) as accounted for. If a neighboring player space is found that player's respective boolean is flagged.
If only one player's boolean is flagged per territory (all the spaces in the holder array) the total elements occupied in the holder array contribute to the player's points. If both
neither is flagged, the territory is still "accounted for" but it doesn't count toward either players score. After all territories are determined, the players scores are tallied up
with all player's territories being adding 1 point for each space owned and each players prisoner count is added for 1 point per each prisoner. After all this, white (having gone
second) is given a 7 point Komi (handicap); 7 was used as it is the standard determined from experience as the usual advantage black has for going first.

	After scoring concludes, the winner is decided and passed to the drawWinner function which renders a message to screen either declaring player 1 the winner, player 2 the winner, or
the match ending in a draw. After 5 seconds of the announcement on screen (an arbitrary lenght decided to be long enough to give recognition to the winner but not long enough to
annoy the players), the game returns to the menu, where the players may select options again.
	
V) Why the Game, Graphics, Menu, gameButtons, and Stone structs?
	For the game, several custom structs were created to compact various related data points and eliminate the necessity of global variables. The game and Graphics structs were created
to handle most major parts of the game and graphics logic holding important pointer information. These two big structs were passed to various functions as pointers to avoid the usage
of global variables; this was chosen as the number of globals would have become messy very quickly and heavy usage of non-constant globals is generally bad practice.

	The game struct is the central struct to the program, it encapsulates the graphics structure, a 2d array of Stone struct, and a couple of miscelanious game logic variables. The 
graphics struct contains all primary graphics pointers (to the renderer, window, various textures) but also the gameButtons, and Menu structs which were used just to bundle all those
related graphical features together. Overall this struct style of organization was suggested by my home school friend I mentioned and adopted as a much more effective, clean, and overall
readable/maintainable option over the globals I used in early versions. Overall this also emphasized the move toward decoupling between frontend and backend (graphics and game logic),
which was also why the game was split by source like it currently is. 

VI) Security Concerns and Warning
	As this application is my first time making anything of this scale or caliber, there are probably several flaws, bugs, or potential risks associated with it. I have identified a 
few, but I firmly believe that the only way someone would run across these was through misuse or irresponsible use of the application. The most apparent example of this is the potential
for save modification and tampering. Since the save format is more or less just written bytes to a what is essentially a .txt file (.mah was just chosen for funsies), a person with
sufficient techncial know-how could tamper with these bytes or even substitute a malicious file that could potentially cause issues in some of the game logic. I warned of this in 
the README.md as well. Additionally, the application will more than likely stop working if the library files or .dll are tampered with and the function WILL stop working if someone 
messes up the .bmp files in the install directory as those are the game textures themselves.

VII) Final Notes
	It's been a fun semester; I thoroughly enjoyed working on this project. Although not as complex as I would have liked (and probably a bit messy), I am very glad I worked on this project.
I apologize for any flaws in my implementation, I promise the build I give works (at least on my computer). If possible if something is horribly wrong please tell me in some way so I can 
correct it and resend it or just for future reference if I decide to revisit this project (this has been a dizzying project afterall). I hope the code is adequately commented.

	