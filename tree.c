#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

#define START_BOARD 0
#define END_BOARD 8

SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src)
{
	int col, row;
	SingleSourceMovesTree* tree;
	tree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	checkAlloc(tree, "tree allocation tree failed");

	tree->source = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	checkAlloc(tree->source, "source allocation failed");

	convertCharTOInt(src, &row, &col);
	//checks which player is it

	if (board[row][col] == 'T')
	{
		buildTree(tree->source, row, col, board, 'T', 0);
		return tree;
	}
	else if (board[row][col] == 'B')
	{
		buildTree(tree->source, row, col, board, 'B', 0);
		return tree;
	}
	else
	{
		return NULL;
	}
}

void convertCharTOInt(checkersPos* src, int* row, int* col)
{
	*row = src->row - 'A';
	*col = src->col - '1';
}

void convertIntTOChar(checkersPos* src, int row, int col)
{
	src->row = row + 'A';
	src->col = col + '1';
}

void buildTree(SingleSourceMovesTreeNode* source, int row, int col, Board board, char player, unsigned short captures) {
	char left, right;
	source->total_captures_so_far = captures;
	checkersPos* location = (checkersPos*)malloc(sizeof(checkersPos));
	checkAlloc(location, "location alloc failed");
	convertIntTOChar(location, row, col);
	source->pos = (checkersPos*)malloc(sizeof(checkersPos));
	checkAlloc(source->pos, "alloc failed");

	source->pos->col = location->col;
	source->pos->row = location->row;
	//copyBoard(source->board, board); //don't know if we need it
	checkNext(source, row, col, board, &left, &right, player);
}

//helper function to build tree
void checkNext(SingleSourceMovesTreeNode* source, int row, int col, Board board, char* pLeft, char* pRight, char player) {
	//works from left to right
	//checks if its possible to go left, right
	//if possible, returns a char that symbols the next possible square
	checkLeft(pLeft, row, col, board, player);
	checkRight(pRight, row, col, board, player);

	source->next_move[0] = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	checkAlloc(source->next_move[0], "next move left alloc failed");
	source->next_move[1] = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	checkAlloc(source->next_move[0], "next move right alloc failed");

	if (player == 'T')
	{
		//decides what the next move is
		sortLeftT(&(source->next_move[0]), pLeft, row + 2, col - 2, board, source->total_captures_so_far, player);
		sortRightT(&(source->next_move[1]), pRight, row + 2, col + 2, board, source->total_captures_so_far, player);
	}
	else
	{
		//change functions here that are in case the player is B
		sortLeftB(&(source->next_move[0]), pLeft, row - 2, col - 2, board, source->total_captures_so_far, player);
		sortRightB(&(source->next_move[1]), pRight, row - 2, col + 2, board, source->total_captures_so_far, player);
	}
}

//checks what letter is in the slot
// :(

void sortLeftT(SingleSourceMovesTreeNode** leftMove, char* pLeft, int row, int col, Board board, unsigned short captures, char player) {
	//if left is out of borders = N
	switch (*pLeft) {
	case 'N':
		*leftMove = NULL;
		break;
	case 'T':
		*leftMove = NULL;
		break;
	case 'B':
		//if the next is square is B, checks if the next next square in the same direction is empty
		//if empty rec call
		if (row < END_BOARD && col >= START_BOARD && board[row][col] == ' ')
		{
			captures++;
			buildTree(*leftMove, row, col, board, player, captures);
		}
		//if its not empty there is no point in doing the recursive call
		else
		{
			*leftMove = NULL;
		}
		break;

		//default is when the square is empty
	default:
		(*leftMove)->pos = (checkersPos*)malloc(sizeof(checkersPos));
		checkAlloc((*leftMove)->pos, "pos alloc failed");
		checkersPos* location = (checkersPos*)malloc(sizeof(checkersPos));
		checkAlloc(location, "location alloc failed");
		if (captures > 0)
		{
			*leftMove = NULL;
		}
		else
		{
			convertIntTOChar(location, row - 1, col + 1);
			(*leftMove)->pos->col = location->col;
			(*leftMove)->pos->row = location->row;
			(*leftMove)->next_move[0] = NULL;
			(*leftMove)->next_move[1] = NULL;
			(*leftMove)->total_captures_so_far = captures;
		}
		break;
	}
}

void sortRightT(SingleSourceMovesTreeNode** rightMove, char* pRight, int row, int col, Board board, unsigned short captures, char player) {
	//if right is out of borders = N
	switch (*pRight) {
	case 'N':
		*rightMove = NULL;
		break;
	case 'T':
		*rightMove = NULL;
		break;
	case 'B':
		//if the next is square is B, checks if the next square in the same direction is empty
		//if empty rec call
		if (row < END_BOARD && col < END_BOARD && board[row][col] == ' ')
		{
			captures++;
			buildTree(*rightMove, row, col, board, player, captures);
		}
		//if its not empty there is no point in doing the recursive call
		else
		{
			*rightMove = NULL;
		}
		break;

		//default is when the square is empty
	default:
		(*rightMove)->pos = (checkersPos*)malloc(sizeof(checkersPos));
		checkAlloc((*rightMove)->pos, "pos alloc failed");
		checkersPos* location = (checkersPos*)malloc(sizeof(checkersPos));
		checkAlloc(location, "location alloc failed");
		if (captures > 0)
		{
			*rightMove = NULL;;
		}
		else {
			convertIntTOChar(location, row - 1, col - 1);
			(*rightMove)->pos->col = location->col;
			(*rightMove)->pos->row = location->row;
			(*rightMove)->next_move[0] = NULL;
			(*rightMove)->next_move[1] = NULL;
			(*rightMove)->total_captures_so_far = captures;
		}

		break;
	}
}

void sortLeftB(SingleSourceMovesTreeNode** leftMove, char* pLeft, int row, int col, Board board, unsigned short captures, char player) {
	//if left is out of borders = N
	switch (*pLeft) {
	case 'N':
		*leftMove = NULL;
		break;
	case 'B':
		*leftMove = NULL;
		break;
	case 'T':
		//if the next is square is B, checks if the next next square in the same direction is empty
		//if empty rec call
		if (row >= START_BOARD && col >= START_BOARD && board[row][col] == ' ')
		{
			captures++;
			buildTree(*leftMove, row, col, board, player, captures);
		}
		//if its not empty there is no point in doing the recursive call
		else
		{
			*leftMove = NULL;
		}
		break;

		//default is when the square is empty
	default:
		(*leftMove)->pos = (checkersPos*)malloc(sizeof(checkersPos));
		checkAlloc((*leftMove)->pos, "pos alloc failed");
		checkersPos* location = (checkersPos*)malloc(sizeof(checkersPos));
		checkAlloc(location, "location alloc failed");
		if (captures > 0)
		{
			*leftMove = NULL;
		}
		else {
			convertIntTOChar(location, row + 1, col + 1);
			(*leftMove)->pos->col = location->col;
			(*leftMove)->pos->row = location->row;
			(*leftMove)->next_move[0] = NULL;
			(*leftMove)->next_move[1] = NULL;
			(*leftMove)->total_captures_so_far = captures;
		}

		break;
	}
}

void sortRightB(SingleSourceMovesTreeNode** rightMove, char* pRight, int row, int col, Board board, unsigned short captures, char player) {
	//if right is out of borders = N
	switch (*pRight) {
	case 'N':
		*rightMove = NULL;
		break;
	case 'B':
		*rightMove = NULL;
		break;
	case 'T':
		//if the next is square is B, checks if the next square in the same direction is empty
		//if empty rec call
		if (row >= START_BOARD && col < END_BOARD && board[row][col] == ' ')
		{
			captures++;

			buildTree(*rightMove, row, col, board, player, captures);
		}
		//if its not empty there is no point in doing the recursive call
		else
		{
			*rightMove = NULL;
		}
		break;

		//default is when the square is empty
	default:
		(*rightMove)->pos = (checkersPos*)malloc(sizeof(checkersPos));
		checkAlloc((*rightMove)->pos, "pos alloc failed");
		checkersPos* location = (checkersPos*)malloc(sizeof(checkersPos));
		checkAlloc(location, "location alloc failed");
		if (captures > 0)
		{
			*rightMove = NULL;
		}
		else {
			convertIntTOChar(location, row + 1, col - 1);
			(*rightMove)->pos->col = location->col;
			(*rightMove)->pos->row = location->row;
			(*rightMove)->next_move[0] = NULL;
			(*rightMove)->next_move[1] = NULL;
			(*rightMove)->total_captures_so_far = captures;
		}

		break;
	}
}

//helper function to checkNext
void checkLeft(char* pLeft, int row, int col, Board board, Player player) {
	if (player == 'T') {
		if (col - 1 < START_BOARD || row + 1 == END_BOARD)
		{
			*pLeft = 'N';
		}
		else {
			*pLeft = board[row + 1][col - 1];
		}
	}
	else {
		if (col - 1 < START_BOARD || row - 1 < START_BOARD)
		{
			*pLeft = 'N';
		}
		else
		{
			*pLeft = board[row - 1][col - 1];
		}
	}
}

//helper function to checkNext
void checkRight(char* pRight, int row, int col, Board board, Player player) {
	if (player == 'T') {
		if (col + 1 == END_BOARD || row + 1 == END_BOARD)
		{
			*pRight = 'N';
		}
		else {
			*pRight = board[row + 1][col + 1];
		}
	}
	else {
		if (col + 1 == END_BOARD || row - 1 < START_BOARD)
		{
			*pRight = 'N';
		}
		else
		{
			*pRight = board[row - 1][col + 1];
		}
	}
}