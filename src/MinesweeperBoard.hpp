/*******************************************************************************
* File:		MinesweeperBoard.hpp
* Author:	Kelley Neubauer
* Date:		8/5/2020
*******************************************************************************/
#ifndef MINESWEEPER_BOARD_HPP
#define MINESWEEPER_BOARD_HPP

#include <vector>
#include "MinesweeperGameStatus.hpp"

class MinesweeperBoard
{
	public:
		MinesweeperBoard(int, int, int);
		~MinesweeperBoard();
		
		// game checks
		bool isInBounds(int, int);
		Status checkGameState();
		bool verifyFlags();

		// setters (modify board)
		void revealCell(int, int);
		void revealRandomCell();
		void revealMines();
		bool flagCell(int, int);
		bool unflagCell(int, int);
		void flipCellFlag(int, int);
		void clearFlags();

		// getters
		std::vector<std::vector<char>> getPlayerBoard();
		char getPlayerBoardCell(int, int);
		int getFlagsRemaining();

		// solver
		bool solvePuzzle();

	private:
		int rows, cols, mines, flagsRemaining;
		std::vector<std::vector<char>> solutionBoard;
		std::vector<std::vector<char>> playerBoard;
		std::vector<std::pair<int, int>> mineCoordinates;
		std::vector<std::pair<int, int>> neighborCellCoordinates;

		// helper functions
		void initializeVect(std::vector<std::vector<char>> &, int, int, char);
		void setNeighborCellCoordinates();
		void setMines();
		void setHints();
};

#endif