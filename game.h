#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 8

typedef struct checkersPos {
	char row, col;
}checkersPos;

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

typedef unsigned char Player;
#include "tree.h"
#include "list.h"

void Turn(Board board, Player player);
void playTurn(SingleSourceMovesList* bestMove, Board board, Player player);
void playGame(Board board, Player starting_player);
void playGameHelper(Board board, Player starting_player, int* movesB, int* movesT, int* mostCaptures, char* playerMostCaptures);
bool printBoard(Board board);