/*******************************************************************************
 * 
 * File:	minesweeperMenus.cpp
 * Author:	Kelley Neubauer
 * Date:	8/8/2020
 * 
 * Description: Implementation file for minesweeper game menus. Uses ncurses in
 *	a self-contained manner.
 * 
 ******************************************************************************/
#include <vector>
#include <ncurses.h>
#include "minesweeperMenus.hpp"

using std::vector;

/**
* 
* MenuChoice mainMenu() 
* 
* Summary: Creates and displays the main menu and gets choice from user.
* 
* Parameters:	none
* 				
* Returns:	   	MenuChoice for the selection made
* 
* Description: Starts and ends ncurses for terminal display. Returns the user
*	choice as a MenuChoice enum (as defined in header) for readability.
* 
**/
MenuChoice mainMenu() 
{
	// start ncurses mode
	initscr();
	noecho();
	cbreak();

	const char gameTitle[] =
		"   __  ________  ___________      _____________  _______ "
		"  /  |/  /  _/ |/ / __/ __/ | /| / / __/ __/ _ \\/ __/ _ \\"
		" / /|_/ // //    / _/_\\ \\ | |/ |/ / _// _// ___/ _// , _/"
		"/_/  /_/___/_/|_/___/___/ |__/|__/___/___/_/  /___/_/|_| "
		"                               by Kelley Neubauer | 2020 "                                                
		"  _/\\_   _/\\_   _/\\_   _/\\_   _/\\_   _/\\_   _/\\_   _/\\_  "
		" <_  _> <_  _> <_  _> <_  _> <_  _> <_  _> <_  _> <_  _> "
		"   \\/     \\/     \\/     \\/     \\/     \\/     \\/     \\/   ";

	vector<const char*> gameMenu = {
		"1. Beginner                                           ",
		"2. Intermediate                                       ",
		"3. Expert                                             ",
		"4. Help                                               ",
		"5. Exit                                               "
	};

	const char menuInstruction[] = "[press enter to make selection]";
	
	int gameTitleHeight = 8;
	int gameTitleWidth = 57;
	int gameTitleStarty = 0;
	// center the menu using terminal window size
	int winMaxx = getmaxx(stdscr);
	int gameTitleStartx = winMaxx / 2 - gameTitleWidth / 2;
	if (gameTitleStartx < 0) {gameTitleStartx = 0;}
	
	int gameMenuHeight = 7;
	int gameMenuWidth = gameTitleWidth;
	int gameMenuStarty = gameTitleStarty + gameTitleHeight;
	int gameMenuStartx = gameTitleStartx;
	
	int menuInstructionHeight = 1;
	int menuInstructionWidth = gameTitleWidth;
	int menuInstructionStarty = gameMenuStarty + gameMenuHeight;
	int menuInstructionStartx  = gameTitleStartx;

	// create ncurses windows
	WINDOW *gameTitleWin = newwin(gameTitleHeight, gameTitleWidth, 
		gameTitleStarty, gameTitleStartx);

	WINDOW *gameMenuWin = newwin(gameMenuHeight, gameMenuWidth, 
		gameMenuStarty, gameMenuStartx);

	WINDOW *menuInstructionWin = newwin(menuInstructionHeight, 
		menuInstructionWidth, menuInstructionStarty, menuInstructionStartx);

	// stylize windows and print graphics
	mvwprintw(gameTitleWin, 0, 0, gameTitle);
	wrefresh(gameTitleWin);
	box(gameMenuWin, 0, 0);
	mvwprintw(menuInstructionWin, 0, 0, menuInstruction);
	wrefresh(menuInstructionWin);

	// get input from gameMenuWin
	keypad(gameMenuWin, true);

	// menu variables
	int highlight = 0;
	int ch = 0;

	while(1) {
		for (size_t i = 0; i < gameMenu.size(); i++) {
			// print all of the items in the menu with selected highlighted
			if (static_cast<int>(i) == highlight) {
				wattron(gameMenuWin, A_REVERSE);
			}
			mvwprintw(gameMenuWin, i + 1, 1, gameMenu[i]);
			wattroff(gameMenuWin, A_REVERSE);
			wrefresh(gameMenuWin);
		}
		wmove(gameMenuWin, highlight + 1, gameTitleWidth - 2);	// move cursor
		
		ch = wgetch(gameMenuWin);	// get user input
		switch(ch) {
			case KEY_UP:
			case 'k':
				highlight -= 1;
				if (highlight == -1) {
					highlight = 0;
				}
				break;
			case KEY_DOWN:
			case 'j':
				highlight += 1;
				if (highlight == 5) {
					highlight = 4;
				}
				break;
			case '1':	
				highlight = 0;
				break;
			case '2':
				highlight = 1;
				break;
			case '3':
				highlight = 2;
				break;
			case '4':	// help
				highlight = 3;
				break;
			case '5':	// quit
				highlight = 4;
				break;
			default:
				break;
		}

		if (ch == 10 || ch == 'g') {	// user has pushed enter or g for go
			if (highlight == 3) {
				helpMenu(gameMenuWidth, gameMenuStarty, gameMenuStartx);

				// reset windows that were erased by help
				mvwprintw(menuInstructionWin, 0, 0, menuInstruction);
				wrefresh(menuInstructionWin);
				box(gameMenuWin, 0, 0);
			}
			else {
				break;
			}
		}
		else if (ch == 'q') {	// user entered q for quit
			highlight = 4;
			break;
		}
	}	// end while(1)
	wclear(gameTitleWin);
	wclear(gameMenuWin);
	wclear(menuInstructionWin);
	
	wrefresh(gameTitleWin);
	wrefresh(gameMenuWin);
	wrefresh(menuInstructionWin);

	delwin(gameTitleWin);
	delwin(gameMenuWin);
	delwin(menuInstructionWin);

	endwin();	// end ncurses

	return static_cast<MenuChoice>(highlight);
}

/**
* 
* void helpMenu(int width, int starty, int startx)
* 
* Summary: Prints the help menu
* 
* Parameters:	int for width 
*				int for starty position
*				int for startx position
* 				
* Returns:	   	void
* 
**/
void helpMenu(int width, int starty, int startx)
{
	vector<const char*> helpMenuText = {
		"[arrow keys] - navigate",
		"[hjkl] - navigate (alternate method using vim keys)",
		"[g] - reveal selected",
		"[r] - reveal random",
		"[f] - flag selected",
		"[v] - verify that flags present valid solution",
		"      does not check correctness",
		"[c] - clear all flags",
		"[s] - solve puzzle", 
		"      does not guarantee a winning solution",
		"[?] - help menu",
		"[q] - quit"
	};

	const char helpInstructionText[] = "[press any key]";
	
	int helpMenuHeight = 
		static_cast<int>(helpMenuText.size()) + 2; // + 2 for border
	int helpMenuWidth = width;
	int helpMenuStarty = starty;
	int helpMenuStartx = startx;

	int menuInstructionHeight = 1;
	int menuInstructionWidth = helpMenuWidth;
	int menuInstructionStarty = helpMenuStarty + helpMenuHeight;
	int menuInstructionStartx  = helpMenuStartx;

	WINDOW *helpMenuWin = newwin(helpMenuHeight, helpMenuWidth, helpMenuStarty, 
		helpMenuStartx);
	WINDOW *helpInstructionWin = newwin(menuInstructionHeight, 
		menuInstructionWidth, menuInstructionStarty, menuInstructionStartx);

	// show contents of the windows
	keypad(helpMenuWin, true);
	box(helpMenuWin, 0, 0);
	for (size_t i = 0; i < helpMenuText.size(); i++) {
		mvwprintw(helpMenuWin, i + 1, 1, helpMenuText[i]);
	} 
	wrefresh(helpMenuWin);

	// print message and hold screen until key is pressed
	mvwprintw(helpInstructionWin, 0, 0, helpInstructionText);
	wrefresh(helpInstructionWin);
	wmove(helpMenuWin, helpMenuHeight - 1, width - 1);
	wgetch(helpMenuWin);

	// clear the screen and delete the windows
	wclear(helpMenuWin);
	wclear(helpInstructionWin);
	wrefresh(helpMenuWin);
	wrefresh(helpInstructionWin);
	delwin(helpMenuWin);
	delwin(helpInstructionWin);
}
