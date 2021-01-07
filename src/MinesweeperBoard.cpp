/*******************************************************************************
 * 
 * File:	MinesweeperBoard.cpp
 * Author:	Kelley Neubauer
 * Date:	8/5/2020
 * 
 * Description: Implementation file for the MinesweeperBoard class. Contains 
 *	code that manages the player and solution game boards during minesweeper 
 *	gameplay.
 * 
 ******************************************************************************/
#include <stddef.h>		// size_t
#include "MinesweeperBoard.hpp"
#include "minesweeperUtils.hpp"

using std::vector;
using std::pair;

/**
* 
* MinesweeperBoard::MinesweeperBoard(int rows, int cols, int mines) 
* 
* Summary: Constructor
* 
* Parameters:	int for board rows
*				int for board cols
*				int for number of mines
* 				
**/
MinesweeperBoard::MinesweeperBoard(int rows, int cols, int mines) 
{
	this->rows = rows;
	this->cols = cols;
	this->mines = this->flagsRemaining = mines;
	
	initializeVect(solutionBoard, rows, cols, '0');
	initializeVect(playerBoard, rows, cols, '-');
	setNeighborCellCoordinates();
	setMines();
	setHints();
}

/**
* 
* MinesweeperBoard::~MinesweeperBoard() 
* 
* Summary: Destructor
* 
**/
MinesweeperBoard::~MinesweeperBoard() 
{

}

/**
* 
* void MinesweeperBoard::initializeVect(vector<vector<char>> &vect, 
*		int rows, int cols, char fillerChar) 
* 
* Summary: Initializes a 2D vector to a specified filler char
* 
* Parameters:	vector of vector of char (reference)
*				int for the number of rows to initialize
*				int for the number of cols to initialize
*				char for the character to fill the vector
* 				
* Returns:	    void
* 
**/
void MinesweeperBoard::initializeVect(vector<vector<char>> &vect, 
		int rows, int cols, char fillerChar) 
{
	for (int i = 0; i < rows; i++) {
		vect.push_back({});
		for (int j = 0; j < cols; j++) {
			vect[i].push_back(fillerChar);
		}
	}
}

/**
* 
* void MinesweeperBoard::setNeighborCellCoordinates() 
* 
* Summary: Initializes a 2D vector to a specified filler char
* 
* Parameters:	none (uses member variables)
* 				
* Returns:	    void
*
* Description: Creates a vector of int pairs to use when iterating through all
*	neighbors of a cell. The vector stores the locations surrounding cell:
*
*	[-1,-1]  [-1,0]  [-1,1]
*	[0, -1]  [cell]  [0, 1]
*	[1, -1]  [1, 0]  [1, 1]
* 
**/
void MinesweeperBoard::setNeighborCellCoordinates()
{
	neighborCellCoordinates.push_back(pair<int, int>(-1, -1));	// up left
	neighborCellCoordinates.push_back(pair<int, int>(-1, 0));	// up
	neighborCellCoordinates.push_back(pair<int, int>(-1, 1));	// up right
	neighborCellCoordinates.push_back(pair<int, int>(0, -1));	// left
	neighborCellCoordinates.push_back(pair<int, int>(0, 1));	// right
	neighborCellCoordinates.push_back(pair<int, int>(1, -1));	// down left
	neighborCellCoordinates.push_back(pair<int, int>(1, 0));	// down
	neighborCellCoordinates.push_back(pair<int, int>(1, 1));	// down right
}

/**
* 
* void MinesweeperBoard::setMines()
* 
* Summary: Sets mines on the board
* 
* Parameters:	none (uses member variables)
* 				
* Returns:	    void
*
* Description: Generates random coordinates using randomNumber helper function 
*	and changes cell from '0' to '*'. Mine locations are also stored in 
*	mineCoordinates vector.
*
*	0 0 0 0 0 0 0 0 0 0 				0 0 0 0 0 0 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				* 0 * * 0 0 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				0 0 0 0 0 0 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				* 0 0 0 0 0 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				0 0 0 0 0 0 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 		->		0 0 0 0 0 0 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				0 0 0 * 0 * 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				0 0 0 0 0 0 * 0 * 0 
*	0 0 0 0 0 0 0 0 0 0 				0 0 * 0 0 0 * 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				0 0 0 0 0 0 0 0 0 0 
* 
**/
void MinesweeperBoard::setMines()
{
	for (int i = 0; i < mines; i++) {
		bool openSpot = false;

		while (!openSpot) {
			int row = randomNumber(0, rows - 1);
			int col = randomNumber(0, cols - 1);

			if (solutionBoard[row][col] == '0') { 
				openSpot = true;
				solutionBoard[row][col] = '*';
				mineCoordinates.push_back(pair<int, int>(row, col));
			}
		}
	}
}

/**
* 
* void MinesweeperBoard::setHints() 
* 
* Summary: Sets hints on the board 
* 
* Parameters:	none (uses member variables)
* 				
* Returns:	    void
*
* Description: Iterates through all mine locations and increments neighboring
*	cells by 1. Each cell will store the total number of neighboring mines.
*
*	0 0 0 0 0 0 0 0 0 0 				1 2 2 2 1 0 0 0 0 0 
*	* 0 * * 0 0 0 0 0 0 				* 2 * * 1 0 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				2 3 2 2 1 0 0 0 0 0
*	* 0 0 0 0 0 0 0 0 0 				* 1 0 0 0 0 0 0 0 0 
*	0 0 0 0 0 0 0 0 0 0 				1 1 0 0 0 0 0 0 0 0
*	0 0 0 0 0 0 0 0 0 0 		->		0 0 1 1 2 1 1 0 0 0 
*	0 0 0 * 0 * 0 0 0 0 				0 0 1 * 2 * 2 2 1 1 
*	0 0 0 0 0 0 * 0 * 0 				0 1 2 2 2 3 * 3 * 1 
*	0 0 * 0 0 0 * 0 0 0 				0 1 * 1 0 2 * 3 1 1 
*	0 0 0 0 0 0 0 0 0 0 				0 1 1 1 0 1 1 1 0 0 
*
* 
**/
void MinesweeperBoard::setHints() 
{
	for (size_t i = 0; i < mineCoordinates.size(); i++) {
		// iterate through all mine coordinates
		int row = mineCoordinates[i].first;
		int col = mineCoordinates[i].second;

		for (size_t j = 0; j < neighborCellCoordinates.size(); j++) {
			// iterate through all neighboring cells
			int rowOffset = neighborCellCoordinates[j].first;
			int colOffset = neighborCellCoordinates[j].second;
			int neighborRow = row + rowOffset;
			int neighborCol = col + colOffset;

			if (isInBounds(neighborRow, neighborCol) 
					&& solutionBoard[neighborRow][neighborCol] != '*') {
				solutionBoard[neighborRow][neighborCol] += 1;
			}
		}
	}
}

/**
* 
* bool MinesweeperBoard::isInBounds(int row, int col) 
* 
* Summary: Checks if a cell is within the bounds of the board
* 
* Parameters:	int for the row of the cell
*				int for the col of the cell
* 				
* Returns:	    bool
*
**/
bool MinesweeperBoard::isInBounds(int row, int col) 
{
	if (row >= 0 && row < this->rows && col >= 0 && col < this->cols) {
		return true;
	}
	return false;
}

/**
* 
* Status MinesweeperBoard::checkGameState()
* 
* Summary: Checks the board to see if the game has been won
* 
* Parameters:	none
* 				
* Returns:	    void
*
* Description: Counts the number of cells that have been revealed on the board
*	to determine when the game has been won. If the number of mines is the same 
*	as the number of cells that are covered, the game is won. If there is a mine
*	showing on the board, the game is lost.
*
**/
Status MinesweeperBoard::checkGameState()
{
	int countRevealed = 0;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (playerBoard[i][j] == '*') {
				return LOSE;
			}
			if (playerBoard[i][j] != '-' && playerBoard[i][j] != '?') {
				countRevealed++;
			}
		}
	}

	// game is won if there are exactly # of mines spaces left covered
	if (countRevealed == (rows * cols - mines)) { 
		return WIN;
	}

	return PENDING;
}

/**
* 
* bool MinesweeperBoard::verifyFlags()
* 
* Summary: Checks the board to see if all flags are in valid positions. Does not
*	check for correctness.
* 
* Parameters:	none
* 				
* Returns:	    bool - true if flags represent a valid arrangemend
*
**/
bool MinesweeperBoard::verifyFlags()
{
	if (flagsRemaining < 0) {
		return false;
	}

	// count neighboring flags for numbered cells
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (playerBoard[i][j] > '0' && playerBoard[i][j] < '9') {
				int surroundingFlags = 0;
				// check all neighbors
				for (size_t k = 0; k < neighborCellCoordinates.size(); k++) {
					// iterate through all neighboring cells
					int rowOffset = neighborCellCoordinates[k].first;
					int colOffset = neighborCellCoordinates[k].second;
					int neighborRow = i + rowOffset;
					int neighborCol = j + colOffset;

					if (isInBounds(neighborRow, neighborCol) 
							&& playerBoard[neighborRow][neighborCol] == '?') {
						surroundingFlags += 1;
					}
				}
				if (surroundingFlags > playerBoard[i][j] - 48) {
					// convert numeric char on board to int for comparison
					return false;
				}
			}
		}
	}
	return true;
}

/**
* 
* void MinesweeperBoard::revealCell(int row, int col) 
* 
* Summary: Reveals cell on board and all neighboring cells 
* 
* Parameters:	int for the row of the cell to uncover
*				int for the col of the cell to uncover
* 				
* Returns:	    void
*
* Description: Uses modified DFS to recursively uncover neighboring cells
*
**/
void MinesweeperBoard::revealCell(int row, int col) 
{
	if (solutionBoard[row][col] == '*') {
		// hit a mine
		playerBoard[row][col] = '*';
		return;
	}
	else if (solutionBoard[row][col] == '0' && playerBoard[row][col]) {
		if (playerBoard[row][col] == '?') {
			flagsRemaining += 1;
		}
		// mark cell as visited
		playerBoard[row][col] = ' ';

		// recursively check all neighbors
		for (size_t j = 0; j < neighborCellCoordinates.size(); j++) {
			// iterate through all neighboring cells
			int rowOffset = neighborCellCoordinates[j].first;
			int colOffset = neighborCellCoordinates[j].second;
			int neighborRow = row + rowOffset;
			int neighborCol = col + colOffset;

			if (isInBounds(neighborRow, neighborCol) 
					&& playerBoard[neighborRow][neighborCol] != ' ') {
				revealCell(neighborRow, neighborCol);
			}
		}
	}
	else if (playerBoard[row][col] == '-') {
		playerBoard[row][col] = solutionBoard[row][col];
	}
}

/**
* 
* void MinesweeperBoard::revealRandomCell() 
* 
* Summary: Reveals a random cell on the board
* 
* Parameters:	none
* 				
* Returns:	    void
*
* Description: Rather than choosing any random cell, which could take an
*	indeterminate amount of time to find a valid move, this function creates
*	a vector of all possible moves and makes a random choice from the moves
*	that are available. This will always take O(n) time worst case.
*
**/
void MinesweeperBoard::revealRandomCell() 
{
	vector<pair<int, int>> possibleMoveCoordinates;

	// add all covered cells as potential moves
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (playerBoard[i][j] == '-') {
				possibleMoveCoordinates.push_back(pair<int, int>(i, j));
			}
		}
	}

	// choose randomly if there is a move to make
	if (!possibleMoveCoordinates.empty()) {
		int randomChoice = randomNumber(0, possibleMoveCoordinates.size() - 1);
		revealCell(possibleMoveCoordinates[randomChoice].first, 
					possibleMoveCoordinates[randomChoice].second);
	}
}

/**
* 
* void MinesweeperBoard::revealMines()
* 
* Summary: Shows all mines on the player board
* 
* Parameters:	none
* 				
* Returns:	    void
*
* Description: Uncovers all mines while leaving flags
*
**/
void MinesweeperBoard::revealMines() 
{
	for (size_t i = 0; i < mineCoordinates.size(); i++) {
		if (playerBoard[mineCoordinates[i].first]
						[mineCoordinates[i].second] != '?') {
			playerBoard[mineCoordinates[i].first]
						[mineCoordinates[i].second] = '*';
		}
	}
}

/**
* 
* bool MinesweeperBoard::flagCell(int row, int col) 
* 
* Summary: Flags a cell and updates flagsRemaining count
* 
* Parameters:	int for the row of the cell
*				int for the col of the cell
* 				
* Returns:	    bool - true if the cell was changed to flag
*
**/
bool MinesweeperBoard::flagCell(int row, int col) 
{
	if (playerBoard[row][col] == '-') {
		flagsRemaining -= 1;
		playerBoard[row][col] = '?';
		return true;
	}
	return false;
}

/**
* 
* bool MinesweeperBoard::unflagCell(int row, int col)
* 
* Summary: Unflags a cell and updates flagsRemaining count
* 
* Parameters:	int for the row of the cell
*				int for the col of the cell
* 				
* Returns:	    bool - true if the cell was changed to unflagged
*
**/
bool MinesweeperBoard::unflagCell(int row, int col)
{
	if (playerBoard[row][col] == '?') {
		flagsRemaining += 1;
		playerBoard[row][col] = '-';
		return true;
	}
	return false;
}

/**
* 
* void MinesweeperBoard::flipCellFlag(int row, int col)
* 
* Summary: Flips a cell from flagged to unflagged or from unflagged to flagged
* 
* Parameters:	int for the row of the cell
*				int for the col of the cell
* 				
* Returns:	    void
*
**/
void MinesweeperBoard::flipCellFlag(int row, int col)
{
	if (playerBoard[row][col] == '-') {
		flagCell(row, col);
	}
	else if (playerBoard[row][col] == '?') {
		unflagCell(row, col);
	}
}

/**
* 
* void MinesweeperBoard::clearFlags()
* 
* Summary: Removes flag from all flagged cells
* 
* Parameters:	int for the row of the cell
*				int for the col of the cell
* 				
* Returns:	    void
*
**/
void MinesweeperBoard::clearFlags()
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (playerBoard[i][j] == '?') {
				flipCellFlag(i, j);
			}
		}
	}
}

/**
* 
* vector<vector<char>> MinesweeperBoard::getPlayerBoard()
* 
* Summary: Returns the player board
* 
* Parameters:	none
* 				
* Returns:	    vector<vector<char>>
*
**/
vector<vector<char>> MinesweeperBoard::getPlayerBoard()
{
	return playerBoard;
}

/**
* 
* char MinesweeperBoard::getPlayerBoardCell(int row, int col)
* 
* Summary: Returns the char value of a particular cell on the board
* 
* Parameters:	int for row of cell
*				int for col of cell
* 				
* Returns:	    char
*
**/
char MinesweeperBoard::getPlayerBoardCell(int row, int col)
{
	if (isInBounds(row, col)) {
		return playerBoard[row][col];
	}
	return 0;
}

/**
* 
* int MinesweeperBoard::getFlagsRemaining()
* 
* Summary: Returns current count of mines less flags on board
* 
* Parameters:	none
* 				
* Returns:	    int for the number of flags remaining
*
**/
int MinesweeperBoard::getFlagsRemaining()
{
	return flagsRemaining;
}

/**
* 
* bool MinesweeperBoard::solvePuzzle()
* 
* Summary: Attempts to make a move that is sure to be correct
* 
* Parameters:	none
* 				
* Returns:	    bool - true if it can find a certain move, false if not
*
* Description: Brute force algorithm for finding a valid move. If it can make
*	a move (or sequence of moves) that it knows to be correct, it will make the 
*	move and return true. If it can't find a certain move it will return false.
*
*	Clear all flags prior to calling unless certain that flags are correct.
*
*	It may take multiple calls to completely solve the puzzle
*
**/
bool MinesweeperBoard::solvePuzzle()
{
	bool moveMade = false;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int coveredNeighbors = 0, flaggedNeighbors = 0;

			// if the cell hase been uncovered and is not a blank,
			// count the neighboring cells that are either uncovered or a flag
			if (playerBoard[i][j] != '-' && playerBoard[i][j] != ' ') {
				for (size_t k = 0; k < neighborCellCoordinates.size(); k++) {
					// iterate through all neighboring cells
					int rowOffset = neighborCellCoordinates[k].first;
					int colOffset = neighborCellCoordinates[k].second;
					int neighborRow = i + rowOffset;
					int neighborCol = j + colOffset;

					if (isInBounds(neighborRow, neighborCol)) {
						if (playerBoard[neighborRow][neighborCol] == '-') {
							coveredNeighbors += 1;
						}
						if (playerBoard[neighborRow][neighborCol] == '?') {
							flaggedNeighbors += 1;
						}
					}
				}

				// if # of covered neighbors + flag neighbors == self
				// add flag to all covered neighbors
				if (((int)playerBoard[i][j] - 48) == (
								coveredNeighbors + flaggedNeighbors)) {
					for (size_t k = 0; k < neighborCellCoordinates.size(); k++) {
						// iterate through all neighboring cells
						int rowOffset = neighborCellCoordinates[k].first;
						int colOffset = neighborCellCoordinates[k].second;
						int neighborRow = i + rowOffset;
						int neighborCol = j + colOffset;

						if (isInBounds(neighborRow, neighborCol)) {
							if(flagCell(neighborRow, neighborCol)) {
								moveMade = true;
							}
						}
					}
				}

				// if # of flag neighbors == self
				// reveal all covered neighbors
				if (((int)playerBoard[i][j] - 48) == (flaggedNeighbors)) {
					for (size_t k = 0; k < neighborCellCoordinates.size(); k++) {
						// iterate through all neighboring cells
						int rowOffset = neighborCellCoordinates[k].first;
						int colOffset = neighborCellCoordinates[k].second;
						int neighborRow = i + rowOffset;
						int neighborCol = j + colOffset;

						if (isInBounds(neighborRow, neighborCol)
								&& playerBoard[neighborRow][neighborCol] == '-') {
							revealCell(neighborRow, neighborCol);
							moveMade = true;
						}
					}
				}	
			} // end uncovered and not blank
		}
	}
	return moveMade;
}
