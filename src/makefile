################################################################################
#	File: 	makefile
#	Author:	Kelley Neubauer
#	Date:	8/5/2020
#
#	Description: makefile for minesweeper
#
################################################################################

#
# Compiler
#
CXX = g++

#
# Compiler Flags
#
CXXFLAGS += -std=c++11 
CXXFLAGS += -Wall 
CXXFLAGS += -pedantic-errors
CXXFLAGS += -g
LDFLAGS += -lncurses
LDFLAGS += -pthread

all : minesweeper

#
# Project Name
#
PROJ += minesweeper

#
# Source Files
#
SRC += minesweeper.cpp 
SRC += MinesweeperGame.cpp
SRC += minesweeperMenus.cpp
SRC += minesweeperUtils.cpp
SRC += MinesweeperBoard.cpp
SRC += MinesweeperDisplay.cpp

#
# Header Files
#
HEADER += MinesweeperGame.hpp
HEADER += minesweeperMenus.hpp
HEADER += minesweeperUtils.hpp
HEADER += MinesweeperBoard.hpp
HEADER += MinesweeperDisplay.hpp
HEADER += MinesweeperGameStatus.hpp

#
# Object Files
#
OBJ += minesweeper.o 
OBJ += MinesweeperGame.o
OBJ += minesweeperMenus.o
OBJ += minesweeperUtils.o
OBJ += MinesweeperBoard.o
OBJ += MinesweeperDisplay.o

#
# Create Executable File
#
${PROJ}: ${OBJ} ${HEADER}
	${CXX} ${LDFLAGS} ${OBJ} -o ${PROJ}

#
# Create Object Files
#
${OBJ}: ${SRC}
	${CXX} ${CXXFLAGS} -c $(@:.o=.cpp)

#
# Clean Up
#
clean:
	rm *.o ${PROJ}