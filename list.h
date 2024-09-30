#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

//single

typedef struct singleSourceMovesListCell {
	checkersPos* position;
	unsigned short captures;
	struct SingleSourceMovesListCell* next;
} SingleSourceMovesListCell;

typedef struct singleSourceMovesList {
	SingleSourceMovesListCell* head;
	SingleSourceMovesListCell* tail;
}SingleSourceMovesList;

//multi

typedef struct multipleSourceMovesListCell {
	SingleSourceMovesList* single_source_moves_list;
	struct multipleSourceMovesListCell* next;
} MultipleSourceMovesListCell;

typedef struct multipleSourceMovesList {
	MultipleSourceMovesListCell* head;
	MultipleSourceMovesListCell* tail;
}MultipleSourceMovesList;

//task 2

SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree);
int optimalMoveHelper(SingleSourceMovesTreeNode* source, SingleSourceMovesList* lst);
void checkAlloc(void* pointer, char txt[]);
void makeEmptyList(SingleSourceMovesList* lst);
void insertDataToStartList(SingleSourceMovesList* lst, int captures, char col, char row);
SingleSourceMovesListCell* createNewListNode(int captures, SingleSourceMovesListCell* next, char col, char row);
void insertNodeToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head);
bool isEmptyList(SingleSourceMovesList* lst);

//task 3

MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);
void makeEmptyListMulti(MultipleSourceMovesList* lst);
void insertDataToEndListMulti(MultipleSourceMovesList* lst, SingleSourceMovesList* data);
MultipleSourceMovesListCell* createNewListNodeMulti(SingleSourceMovesList* data, MultipleSourceMovesListCell* next);
void insertNodeToEndListMulti(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* tail);
bool isEmptyListMulti(MultipleSourceMovesList* lst);
