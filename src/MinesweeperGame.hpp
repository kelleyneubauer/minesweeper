/*******************************************************************************
* File:		MinesweeperGame.hpp
* Author:	Kelley Neubauer
* Date:		8/5/2020
*******************************************************************************/
#ifndef MINESWEEPER_GAME_HPP
#define MINESWEEPER_GAME_HPP

#include <ctime>	// time_t
#include <mutex>
#include "MinesweeperBoard.hpp"
#include "MinesweeperDisplay.hpp"
#include "MinesweeperGameStatus.hpp"

class MinesweeperGame
{
	public:
		MinesweeperGame(int, int, int);
		~MinesweeperGame();

		void startGame();

	private:
		MinesweeperBoard *gameBoard;
		MinesweeperDisplay *gameDisplay;
		Status gameState;
		time_t startTime;
		std::mutex gameStateMtx;

		// helper functions
		void timeKeeper();
		void moveCursorOnBoard(int, int);
		void setGameState(Status);
		Status getGameState();
};

#endif