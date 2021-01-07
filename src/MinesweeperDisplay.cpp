/*******************************************************************************
 * 
 * File:	MinesweeperDisplay.cpp
 * Author:	Kelley Neubauer
 * Date:	8/5/2020
 * 
 * Description: Implementation file for the MinesweeperDisplay class. Contains 
 *	code that manages the input/ output of minesweeper display instance using 
 *	ncurses.
 * 
 ******************************************************************************/
#include <stddef.h>		// size_t
#include "MinesweeperDisplay.hpp"
#include "minesweeperMenus.hpp"

using std::vector;

/**
* 
* MinesweeperDisplay::MinesweeperDisplay(int rows, int cols, int mines) 
* 
* Summary: Constructor
* 
* Parameters:	int for board rows
*				int for board cols
*				int for number of mines
* 				
**/
MinesweeperDisplay::MinesweeperDisplay(int rows, int cols) 
{
	this->rows = rows;
	this->cols = cols;
	cursorRow = 0;
	cursorCol = 0;
	topBorder = 1;
	bottomBorder = 1;
	leftBorder = 2;
	rightBorder = 1;
	colDisplayOffset = 2;
	
	// start ncurses mode
	initscr();
	noecho();
	cbreak();

	// create game board window
	int gameBoardWinHeight = rows + topBorder + bottomBorder;
	int gameBoardWinWidth = (cols * colDisplayOffset) 
									+ leftBorder + rightBorder;
	int gameBoardWinStarty = 0;
	int gameBoardWinStartx = 0;
	this->gameBoardWin = newwin(gameBoardWinHeight, gameBoardWinWidth, 
							gameBoardWinStarty, gameBoardWinStartx);
	keypad(gameBoardWin, true);
	box(gameBoardWin, 0, 0);

	// create flags display window
	int flagsWinHeight = 1;
	int flagsWinWidth = 5;
	int flagsWinStarty = gameBoardWinStarty + gameBoardWinHeight;
	int flagsWinStartx = gameBoardWinStartx + 1;
	this->flagsWin = newwin(flagsWinHeight, flagsWinWidth, flagsWinStarty, 
								flagsWinStartx);

	// create timer display window
	int timerWinHeight = 1;
	int timerWinWidth = 5;
	int timerWinStarty = gameBoardWinStarty + gameBoardWinHeight;
	int timerWinStartx = gameBoardWinWidth - timerWinWidth;
	this->timerWin = newwin(timerWinHeight, timerWinWidth, timerWinStarty, 
								timerWinStartx);

	// create message window
	int gameMessageWinHeight = 15;
	int gameMessageWinWidth = 100;
	int gameMessageWinStarty = flagsWinStarty + flagsWinHeight + 1;
	int gameMessageWinStartx = gameBoardWinStartx + 1;
	this->gameMessageWin = newwin(gameMessageWinHeight, gameMessageWinWidth, 
							gameMessageWinStarty, gameMessageWinStartx);
	keypad(gameMessageWin, true);
}

/**
* 
* MinesweeperDisplay::~MinesweeperDisplay() 
* 
* Summary: Destructor
* 
**/
MinesweeperDisplay::~MinesweeperDisplay() 
{
	wclear(gameBoardWin);
	wclear(timerWin);
	wclear(flagsWin);
	wclear(gameMessageWin);

	wrefresh(gameBoardWin);
	wrefresh(timerWin);
	wrefresh(flagsWin);
	wrefresh(gameMessageWin);

	delwin(gameBoardWin);
	delwin(timerWin);
	delwin(flagsWin);
	delwin(gameMessageWin);

	endwin();	// end ncurses mode
}

/**
* 
* int MinesweeperDisplay::getUserInput()
* 
* Summary: Returns keyboard input from the game board
* 
* Parameters:	none
* 				
* Returns:	    int
*
**/
int MinesweeperDisplay::getUserInput()
{
	return(wgetch(gameBoardWin));
}

/**
* 
* void MinesweeperDisplay::printBoard(const vector<vector<char>> &vect)
* 
* Summary: Prints a 2D vector to the game board window.
* 
* Parameters:	vector of vector of char
* 				
* Returns:	    void
*
* Description: Ncurses is not thread safe - the work around used here is to
*	limit printing to the display to one thread at a time using a mutex.
*
**/
void MinesweeperDisplay::printBoard(const vector<vector<char>> &vect)
{
	displayMtx.lock();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (vect[i][j] == '?') {
				wattron(gameBoardWin, A_REVERSE);	// invert color for flags
			}
			mvwprintw(gameBoardWin, i + topBorder, 
						(j * colDisplayOffset) + leftBorder,
						"%c",
						vect[i][j]);

			if (vect[i][j] == '?') {
				wattroff(gameBoardWin, A_REVERSE);	// turn off invert color
			}
		}
	}
	displayMtx.unlock();
	setCursorPosition(cursorRow, cursorCol);	// return cursor after printing
}

/**
* 
* void MinesweeperDisplay::printFlags(int flagCount)
* 
* Summary: Moves the cursor to a position on the board
* 
* Parameters:	int for the row
*				int for the col
* 				
* Returns:	    void
*
* Description: Ncurses is not thread safe - the work around used here is to
*	limit printing to the display to one thread at a time using a mutex.
*
**/
void MinesweeperDisplay::printFlags(int flagCount)
{
	displayMtx.lock();
	wclear(flagsWin);
	mvwprintw(flagsWin, 0, 0, "%d", flagCount);
	wrefresh(flagsWin);
	displayMtx.unlock();
	setCursorPosition(cursorRow, cursorCol);
}

/**
* 
* void MinesweeperDisplay::printTime(int time)
* 
* Summary: Calculates and prints the time
* 
* Parameters:	int for the time to print
* 				
* Returns:	    void
*
* Description: Uses an offset so that the time is right aligned. Prints 9999
* 	for all times > 9999.
*	Ncurses is not thread safe - the work around used here is to
*	limit printing to the display to one thread at a time using a mutex.
*	This function uses a non-blocking try to lock mutex and will skip printing
*	the updated time if locked.
*
**/
void MinesweeperDisplay::printTime(int time)
{
	int digitCalc = time;
	int timeOffset = 3;	// starting point for single digit number
	while (digitCalc /= 10) {
		timeOffset--;
	}
	
	if (timeOffset < 0) {
		timeOffset = 0;
		time = 9999;
	}

	if (displayMtx.try_lock()) {	// skip updating if the mutex is locked
		wclear(timerWin);
		mvwprintw(timerWin, 0, timeOffset, "%d", time);
		wrefresh(timerWin);
		displayMtx.unlock();

		setCursorPosition(cursorRow, cursorCol);
	}
}

/**
* 
* void MinesweeperDisplay::printGameMessage(const char *message)
* 
* Summary: Prints text to the message window
* 
* Parameters:	char* for the message to print
* 				
* Returns:	    void
*
* Description: The mutex is outside of the critical section here so the cursor
*	stays put doesn't jump around. I'm sure this could be improved.
*
**/
void MinesweeperDisplay::printGameMessage(const char *message)
{	
	displayMtx.lock();
	mvwprintw(gameMessageWin, 0, 0, message);
	wprintw(gameMessageWin, "\n\n[press enter]");
	
	int ch;
	while (1)	// hold screen until enter or q is pressed
	{
		ch = wgetch(gameMessageWin);
		if (ch == 10 || ch == 'q') {
			break;
		}
	}
	
	wclear(gameMessageWin);
	wrefresh(gameMessageWin);
	displayMtx.unlock();
	setCursorPosition(cursorRow, cursorCol);
}

/**
* 
* void MinesweeperDisplay::setCursorPosition(int row, int col) 
* 
* Summary: Moves the cursor to a position on the board
* 
* Parameters:	int for the row
*				int for the col
* 				
* Returns:	    void
*
* Description: Ncurses is not thread safe - the work around used here is to
*	limit printing to the display to one thread at a time using a mutex.
*
**/
void MinesweeperDisplay::setCursorPosition(int row, int col) 
{
	cursorRow = row;
	cursorCol = col;
	displayMtx.lock();
	wmove(gameBoardWin, row + topBorder, col * colDisplayOffset + leftBorder);
	wrefresh(gameBoardWin);
	displayMtx.unlock();
}

/**
* 
* int MinesweeperDisplay::getCursorRow()
* 
* Summary: Returns current row of the cursor
* 
* Parameters:	none
* 				
* Returns:	    int
*
**/
int MinesweeperDisplay::getCursorRow()
{
	return cursorRow;
}

/**
* 
* int MinesweeperDisplay::getCursorCol()
* 
* Summary: Returns current col of the cursor
* 
* Parameters:	none
* 				
* Returns:	    int
*
**/
int MinesweeperDisplay::getCursorCol()
{
	return cursorCol;
}
