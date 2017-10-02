//This program improves the base Battleship program given.


#include <cstdlib>
#include <iostream>
#include "bot.h"
#include "screen.h"

using namespace std;

int ROWS;
int COLS;

int iter = 0;

/* Initialization procedure, called when the game starts:

   init (rows, cols, num, screen, log) 
 
   Arguments:
    rows, cols = the boards size
    num        = the number of ships 
    screen     = a screen to update your knowledge about the game
    log        = a cout-like output stream
*/
void init(int rows, int cols, int num, Screen &screen, ostream &log) 
{
	ROWS = rows;
	COLS = cols;
	log << "Start." << endl;
}


/* The procedure handling each turn of the game:
 
   next_turn(sml, lrg, num, gun, screen, log)
 
   Arguments:
    sml, lrg = the sizes of the smallest and the largest ships that are currently alive
    num      = the number of ships that are currently alive
    gun      = a gun.
               Call gun.shoot(row, col) to shoot: 
                  Can be shot only once per turn. 
                  Returns MISS, HIT, HIT_N_SUNK, ALREADY_HIT, or ALREADY_SHOT.
    screen   = a screen to update your knowledge about the game
    log      = a cout-like output stream
*/
void next_turn(int sml, int lrg, int num, Gun &gun, Screen &screen, ostream &log)
{

	int r = iter / COLS;
	int c = iter % COLS;
	iter+=1;
	log << "Smallest: " << sml << " Largest: " << lrg << ". ";
	log << "Shoot at " << r << " " << c << endl;

	result res = gun.shoot(r, c);

	// add result on the screen
	if (res == MISS) {
		screen.mark(r, c, 'x', BLUE); 
		if((screen.read(r, c-1) == '@') && (screen.read(r, c-2) == 'x')){ //If there is a vertical ship behind the spot hit
			screen.mark(r+1, c, 'x', BLUE); //Marks the spots next to where the vertical ship will continue, the row under, as 'x'	
			screen.mark(r+1, c-2, 'x', BLUE);
		}
		for(int i = c+1; (screen.read(r, i) == 'x') ; i++) { //If the following spot(s) are already marked 'x', then skip checking them 
			iter +=1;
		}
	}

	else if (res == HIT) {
		screen.mark(r, c, '@', GREEN); 
		if(screen.read(r, c-1) == '@'){ //If the ship is horizontal (was hit directly left)
			screen.mark(r+1, c-1, 'x', BLUE); //Marks spots directly below '@' as 'x'
			screen.mark(r+1, c, 'x', BLUE);	
		}
		else if(screen.read(r-1,c) == '@'){ //If the ship is vertical (was hit one above)
			screen.mark(r, c+1, 'x', BLUE); //Marks spot right of ship and spots next to where the vertical ship will continue, the row under, as 'x'
			screen.mark(r+1, c-1, 'x', BLUE);
			screen.mark(r+1, c+1, 'x', BLUE);
		}
		for(int i = c+1; (screen.read(r, i) == 'x') ; i++) { //If the following spot(s) are already marked 'x', then skip checking them 
			iter +=1;
		}
	}

	else if (res == HIT_N_SUNK) {
		screen.mark(r, c, 'S', RED); 
		screen.mark(r+1, c, 'x', BLUE); //Marks spots directly right and below as 'x'
		screen.mark(r, c+1, 'x', BLUE);
		if(((screen.read(r,c-1) == '@') && (screen.read(r, c-2) == 'x')) || ((screen.read(r,c-1) == '@') && (c-1 == 0))) { // If ship is length 2 (addresses exception from res == HIT) 
			screen.mark(r+1, c-1, 'x', BLUE); //Marks spots directly below previous '@' as 'x'
		}
		for(int i = c+1; (screen.read(r, i) == 'x') ; i++) { //If the following spot(s) are already marked 'x', then skip checking them 
			iter +=1;
		}
	}
}


/*
Programmer Documentation
In order to improve battleship program, the following improvements were implemented:
1) When two horizontal '@' spots are hit consecutively, 'x's are placed on the board below each one, because no ship can be there. 
2) When a ship is sunk, and 'S' is on the board, the spot to the right and the spot below it are marked 'x' because no ship can be there.
3) After the gun shoots, if there is an 'x' directly to the right, the iterator will +1 to skip over rechecking the spot, and will continue to +1 if there are consecutive 'x's in the row.
-Several other spots were able to be marked 'x' based on whether the ship was seen to continue vertically or horizontally.
*/
