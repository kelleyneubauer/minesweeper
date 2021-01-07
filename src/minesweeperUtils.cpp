/*******************************************************************************
* File:		minesweperUtils.cpp
* Author:	Kelley Neubauer
* Date:		8/5/2020
* 
* Description: This file contains utility functions for minesweeper game
* 
*******************************************************************************/
#include <cstdlib> 	//rand

/**
* 
* int randomNumber(int min, int max)
* 
* Summary: Generates a random num between min (inclusive) and max (inclusive)
* 
* Parameters:	an int for min number in range
* 				an int for max number in range
* 				
* Returns:	    an int
* 
* Description: Uses seeded rand to generate number. srand should be called 
*		elsewhere (once in the program) for best results -- e.g. srand(time(0))
* 
**/
int randomNumber(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}
