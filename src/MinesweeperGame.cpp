/*******************************************************************************
 * 
 * File:	MinesweeperGame.cpp
 * Author:	Kelley Neubauer
 * Date:	8/5/2020
 * 
 * Description: Implementation file for the MinesweeperGame class. Contains code 
 *	that runs the playable game. It interacts with a MinesweeperBoard object to 
 *	manage the game and a MinesweeperDisplay object to display the game.
 * 
 ******************************************************************************/
#include <vector>
#include <ncurses.h>
#include <thread>
#include <unistd.h>		// sleep
#include "MinesweeperGame.hpp"

using std::vector;

/**
* 
* MinesweeperGame::MinesweeperGame() 
* 
* Summary: Constructor
*
* Parameters:	int for board rows
*				int for board cols
*				int for number of mines
* 
**/
MinesweeperGame::MinesweeperGame(int rows, int cols, int mines) 
{
	srand(time(0));	// seed for random number generation
	setGameState(PENDING);
	gameBoard = new MinesweeperBoard(rows, cols, mines);
	gameDisplay = new MinesweeperDisplay(rows, cols);
}

/**
* 
* MinesweeperGame::~MinesweeperGame() 
* 
* Summary: Destructor. Frees dynamically allocated memory 
* 
**/
MinesweeperGame::~MinesweeperGame() 
{
	delete(gameBoard); 
	delete(gameDisplay);
}

/**
* 
* void MinesweeperGame::startGame() 
* 
* Summary: Starts the minesweeper game
* 
* Parameters:	none
* 				
* Returns:	   	void
* 
* Description: Prints the board and flags. Starts the timer in a new thread.
*	gets user input until the game state changes or user quits. Requests changes 
*	to the board based on input and modifes display.
*
* Reference:
*	using threads in a class: https://stackoverflow.com/questions/17472827/
* 
**/
void MinesweeperGame::startGame() 
{
	gameDisplay->printBoard(gameBoard->getPlayerBoard());
	gameDisplay->printFlags(gameBoard->getFlagsRemaining());

	// start the timer on a new thread
	std::thread timeThread(&MinesweeperGame::timeKeeper, this);

	while (getGameState() == PENDING) {
		int ch = gameDisplay->getUserInput();

		switch (ch) {
			case KEY_LEFT:			
			case 'h':
				moveCursorOnBoard(0, -1);
				break;
			case KEY_RIGHT:		
			case 'l':
				moveCursorOnBoard(0, 1);
				break;
			case KEY_UP:		
			case 'k':
				moveCursorOnBoard(-1, 0);
				break;
			case KEY_DOWN:		
			case 'j':
				moveCursorOnBoard(1, 0);
				break;
			case 'g':		// reveal cell
				gameBoard->revealCell(gameDisplay->getCursorRow(), 
						gameDisplay->getCursorCol());
				setGameState(gameBoard->checkGameState());
				gameDisplay->printBoard(gameBoard->getPlayerBoard());
				break;
			case 'r':		// reveal random cell
				gameDisplay->printGameMessage("Computer is making a guess");
				gameBoard->revealRandomCell();
				setGameState(gameBoard->checkGameState());
				gameDisplay->printBoard(gameBoard->getPlayerBoard());
				break;
			case 'f':		// flag cell
				gameBoard->flipCellFlag(gameDisplay->getCursorRow(), 
						gameDisplay->getCursorCol());
				gameDisplay->printBoard(gameBoard->getPlayerBoard());
				gameDisplay->printFlags(gameBoard->getFlagsRemaining());
				break;
			case 'v':		// verify flags 
				if (gameBoard->verifyFlags()) {
					gameDisplay->printGameMessage("Flags are valid");
				}
				else {
					gameDisplay->printGameMessage("Flags are not valid");
				}
				break;
			case 'c':		// clear flags
				gameBoard->clearFlags();
				gameDisplay->printBoard(gameBoard->getPlayerBoard());
				gameDisplay->printFlags(gameBoard->getFlagsRemaining());
				break;
			case 's':		// solve puzzle
				gameDisplay->printGameMessage("Computer will attempt to solve puzzle");
				gameBoard->clearFlags();
				while (getGameState() == PENDING) {
					if (!gameBoard->solvePuzzle()) {
						gameDisplay->printGameMessage(
								"Computer is making a guess");
						gameBoard->revealRandomCell();
					}
					setGameState(gameBoard->checkGameState());
					gameDisplay->printBoard(gameBoard->getPlayerBoard());
					gameDisplay->printFlags(gameBoard->getFlagsRemaining());
				}
				break;
			case '?':		// help
				gameDisplay->printGameMessage(					
					"[arrow keys] - navigate\n"
					"[hjkl] - navigate (alternate method using vim keys)\n"
					"[g] - reveal selected\n"
					"[r] - reveal random\n"
					"[f] - flag selected\n"
					"[v] - verify that flags present valid solution\n"
					"      does not check correctness\n"
					"[c] - clear all flags\n"
					"[s] - solve puzzle\n"
					"      does not guarantee a winning solution\n"
					"[?] - help menu\n"
					"[q] - quit");
				break;
			default:
				break;
		}

		if (ch == 'q') {	// user has quit game
			setGameState(LOSE);
			break;
		}
	}

	// game completion messages
	if (getGameState() == WIN) {
		gameDisplay->printGameMessage("YOU WIN!");
	}
	else if (getGameState() == LOSE) {
		gameBoard->revealMines();
		gameDisplay->printBoard(gameBoard->getPlayerBoard());
		gameDisplay->printGameMessage("YOU LOSE :(");
	}

	timeThread.join();
}

/**
* 
* void MinesweeperGame::timeKeeper()
* 
* Summary: Tracks the game time and prints to display once per second
* 
* Parameters:	none
* 				
* Returns:	    void
*
* Description: Runs continuously in a loop until the game state changes - it
*	should only be called by a separate thread or process.
*
**/
void MinesweeperGame::timeKeeper()
{
	time_t start, current;
	double elapsed = 0;
	time(&start);

	while (getGameState() == PENDING) {
		elapsed = difftime(time(&current), start);
		gameDisplay->printTime(elapsed);
		sleep(1);
	} 
}

/**
* 
* void MinesweeperGame::moveCursorOnBoard(int rowChange, int colChange)
* 
* Summary: Moves the cursor based on user input
* 
* Parameters:	int for the row change from current
*				int for the col change from current
* 				
* Returns:	    void
*
* Description: Checks if the potential move is in bounds and moves cursor
*	accordingly.
*
**/
void MinesweeperGame::moveCursorOnBoard(int rowChange, int colChange)
{
	if (gameBoard->isInBounds(gameDisplay->getCursorRow() + rowChange, 
				gameDisplay->getCursorCol() + colChange)) {
		gameDisplay->setCursorPosition(gameDisplay->getCursorRow() + rowChange, 
				gameDisplay->getCursorCol() + colChange);
	}
}

/**
* 
* void MinesweeperGame::setGameState(Status newStatus)
* 
* Summary: Modifies gameState member variable
* 
* Parameters:	Status for the new game state
* 				
* Returns:	    void
*
* Description: This function is used to modify a shared resource between threads
*
**/
void MinesweeperGame::setGameState(Status newStatus)
{
	gameStateMtx.lock();
	gameState = newStatus;
	gameStateMtx.unlock();
}

/**
* 
* Status MinesweeperGame::getGameState()
* 
* Summary: Returns gameState member variable
* 
* Parameters:	none
* 				
* Returns:	    Status for the current game state
*
* Description: This function is used to access a shared resource between threads
*
**/
Status MinesweeperGame::getGameState()
{
	gameStateMtx.lock();
	Status currentState = gameState;
	gameStateMtx.unlock();
	
	return currentState;
}
