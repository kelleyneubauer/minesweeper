/*******************************************************************************
* File:		MinesweeperDisplay.hpp
* Author:	Kelley Neubauer
* Date:		8/5/2020
*******************************************************************************/
#ifndef MINESWEEPER_DISPLAY_HPP
#define MINESWEEPER_DISPLAY_HPP

#include <ncurses.h>
#include <vector>
#include <mutex>

class MinesweeperDisplay
{
	public:
		MinesweeperDisplay(int, int);
		~MinesweeperDisplay();

		// input
		int getUserInput();

		// output
		void printBoard(const std::vector<std::vector<char>> &);
		void printFlags(int);
		void printTime(int);
		void printGameMessage(const char*);

		// setters
		void setCursorPosition(int, int);

		// getters
		int getCursorRow();
		int getCursorCol(); 

	private:
		int rows, cols;
		int leftBorder, rightBorder, topBorder, bottomBorder;
		int rowDisplayOffset, colDisplayOffset;	
		int row, col, ch, cursorRow, cursorCol;
		WINDOW *gameBoardWin, *timerWin, *flagsWin, *gameMessageWin;
		std::mutex displayMtx;
};

#endif