I) Foreword
	As a note to any potential users of the MahoGO application, this application is first and foremost and Windows application. It was built in Visual Studio and tested primarily
with the VC++ compiler. As such, there is no guarantee the application may work outside of a Windows environment - further it is unlikely it will work as intended if at all outside
of a Windows environment. Additionally, the application was built on a system running Windows 10, ideally the application would be used in a similar environment, but it should, in
theory work with earlier or later versions of windows as long as SDL2 dependicides are not changed as a result. There are no guarantees, as with most applications, that MahoGO will
run in a WINE environment. This application was built using the SDL2 library for windows 32 bit (x86) architectures. The ideal environment to run MahoGO, therefore, would be any 
computer running Windows 10. The application is also written to render in very specific resolutions; I apologize for this restriction, but there wasn't enough time to implement
dynamic resolution support. The window itself renders in 1180x960 p, so I recommend a relatively large display with at least 1920x1080 in resolution; if you do not have an adquately
large screen, the game board or window as a whole will probably not display.

II) What is MahoGO?
	MahoGO is a Windows desktop application that allows two players at the same computer to play against one another in the board game Go. If you do not know, Go is one of the oldest
games in human history, having its origins in Ancient China. The rules are relatively simple and the overall goal is to capture territory on a 19x19 board by encircling spaces with 
your own stones. If you would like a better explanation for the game itself or how to play, I recommend doing some research online. 
I particularly recommend starting here: https://www.britgo.org/intro/intro2.html 
	Note the rules that MahoGO obey may differ slightly from what some players are used to. There aredifferent variations of some of Go's rules; in particular rules of Ko and 
the way scoring/counting work vary from ruleset to ruleset. MahoGO operates on predominantly Japanese versionsof these rules, in particular the Japanese rule of Ko and Japanese 
(territory) scoring is employed. A 7 point Komi (handicap) is given to white; this is standard in tournaments as 7 points is the usual advantage black has for going first.

III) Setup
	Option A) Source Compilation (Good Luck)
		The first option for setting up the application is to compile the source from scratch. The zip file downloaded should contain all the necessary files to do so.
	There are 3 source files in total: Main.c, graphics.c, and game.c. In addition to the source, there are 2 header files: graphics.h and game.h. The application as stated above is
	dependent on SDL2 and so the SDL2.dll should be in the installation directory along with the source. Since a .exe is provided, compilation is not necessary. If you wish to compile,
	however, the SDL libs (for linking) are in the lib subdirectory; also keep in mind the .exe was compiled using VC++ through Visual Studio, although other compilers should work.
	Compilation should be done through a command prompt or equivalent. Once compiled, you may run the executable either from the command prompt or by opening it in a file explorer. 
	You may wish to create a shortcut to the executable for ease of access. Once compiled and any setup is done, you may continue to the usage/playing section (section IV) of the 
	README.
	
	Option B) Provided Executable (Easy Route)
		This is by far the simplest option for application usage. Within the downloaded zip file, a pre-compiled executable (.exe format) shoud exist. You should be able use it directly
	from the directory or from the command prompt at will. If you are not running Windows 10, it may not fully work, but this is unlikely for any Windows user. WINE may also work, 
	but this is untested.
	
IV) Usage
	To begin playing MahoGO start up the executable (or equivalent if this does actually compile on Linux/OSX systems). Upon startup, you should find yourself in the main menu.
In the main menu, there are 3 options: New Game, Load Game, and Exit. New Game creates a new game; this creates a blank board and starts the turncount and prisoner counts at 0.
Load game loads a previously saved game; this includes the board status, turncount, and prisoner counters. There can only be one saved game at time. This save appears in the install
directory as save.mah. Please do not modify the save with external applications or try to manually replace save files with other save files; this could have unintended consequences.

	Once you have either loaded a previous game or started a new game, a 19x19 board with a side menu should appear. Black goes first alternating back and forth with white. To place
a stone on the board, left click the intersection on the board you want to place on. You cannot place a stone in an already occupied space or in a space that would leave one of your
own stones without liberties. There is no move reversal or confirmation of moves, so choose and click carefully. Captures of enemies are handled automatically and the atari rule is
also in play. On the side bar, 3 buttons should be available: Pass, Resign, and Save. Pass passes the current player's turn, no point penalties are assigned for this. Resign resigns
the current player from the matches; the opposing player will automatically win as expected. Save saves the current game state for later loads. Saving the game does not exit the game
and you can save multiple times per game, but each save overwrites the previous one. The fourth blank rectangle is a status announcement space. It will state whether a move you try to
make is invalid or complement the player when a valid move is made. 

	To exit a game, the players have 3 options. If both players pass consecutively, the game ends, going into a scoring phase. If one player resigns, the game ends, no scoring is
necessary. Thirdly pressing the window "X" will immediately exit the application, no scoring or winner announcement will occur. If the game ends with both players passing, a scoring
phase begins. At the start of the scoring phase, the players will select all "dead" stones on the board. "Dead" stones are those that are in an opponent's territory that would be
captured if the game continued to progress. Simply left clicking on stone's that are dead will remove them from the board. Be careful with clicking stones; make sure both players 
agree a stone is dead and the right stone is selected. If the players fail to remove dead stones, territories rightfully captured by a player will not be recognized by the program.
Once all dead stones have been removed, click the "Continue" button on the right bar; it should have taken the place of the save button. Upon clicking continue, the game will 
calculate both players scores. The game begins by determining how much territory each player controls and then adds to it the total number of prisoners a player has captured. At the
end of this, white's handicap (komi) of 7 points is added. The game then determines who has the greater number of points and announces the winner or if the game conlcuded in a draw.
At the conclusion of a game, MahoGO should go back to the menu.

V) Warnings
	To discourage improper use of the application, this section should outline a few things to look out for or explicity state what not to do with the application. 

	As stated above, DO NOT modify, replace, or otherwise tamper with the game's save file. The save file is located in the install directory under the name "save.mah". Tampering
with the file could have unforseen consequences on the game itself and potentially open up security risks for the user. Don't say you weren't warned. In addition to the save file,
DO NOT modify or otherwise tamper with the game assets, this includes the SDL or other dependent libraries, any source files, and .dll or any .bmp files. Bitmap files (.bmp) are used
for texture storage, if you delete or otherwise modify them, this could cause the game to stop working or potentially more devastating consequences. Tampering with libraries or 
especially dynamic linked libraries (.dll) will more than likely break the application, as it depends on those files to operate.

	