/*******************************************************************************
* File:		minesweeper.cpp
* Author:	Kelley Neubauer
* Date:		8/5/2020
* 
* Description: 
* 
* 	This file contains code that creates and runs an instance of Minesweeper
* 
*******************************************************************************/
#include "minesweeperMenus.hpp"
#include "MinesweeperGame.hpp"

// beginner: 10x10, 10 mines
#define BEGINNER_ROWS 		10
#define BEGINNER_COLS 		10
#define BEGINNER_MINES 		10
// intermediate: 16x16, 40 mines
#define INTERMEDIATE_ROWS 	16
#define INTERMEDIATE_COLS 	16
#define INTERMEDIATE_MINES 	40
// expert: 16x40, 99 mines
#define EXPERT_ROWS 		16
#define EXPERT_COLS 		40
#define EXPERT_MINES 		99

int main()
{
	while (1) {
		MinesweeperGame *newGame = nullptr;

		// run menu and process user choice
		MenuChoice choice = mainMenu();
		switch (choice) {
			case BEGINNER:		
				newGame = new MinesweeperGame(
					BEGINNER_ROWS, BEGINNER_COLS, BEGINNER_MINES);
				break;
			case INTERMEDIATE:		
				newGame = new MinesweeperGame(
					INTERMEDIATE_ROWS, INTERMEDIATE_COLS, INTERMEDIATE_MINES);
				break;	
			case EXPERT:		
				newGame = new MinesweeperGame(
					EXPERT_ROWS, EXPERT_COLS, EXPERT_MINES);
				break;
			default:
				break;
		}

		if (choice == EXIT) {
			break;
		}
		else {
			newGame->startGame();
		}
		
		if (newGame) {
			delete(newGame);
		}
	}

	return 0;
}