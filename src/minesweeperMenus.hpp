/*******************************************************************************
* File:		minesweeperMenus.hpp
* Author:	Kelley Neubauer
* Date:		8/5/2020
*******************************************************************************/
#ifndef MINESWEEPER_MENUS_HPP
#define MINESWEEPER_MENUS_HPP

enum MenuChoice {BEGINNER, INTERMEDIATE, EXPERT, HELP, EXIT};

MenuChoice mainMenu();
void helpMenu(int, int, int);

#endif