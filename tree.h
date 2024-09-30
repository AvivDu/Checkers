#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct singleSourceMovesTreeNode {
	Board board;
	checkersPos* pos;
	unsigned short total_captures_so_far;// times jumped
	struct SingleSourceMoveTreeNode* next_move[2]; // destination moves
}SingleSourceMovesTreeNode;

typedef struct singleSourceMovesTree {
	SingleSourceMovesTreeNode* source;
}SingleSourceMovesTree;

SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src);
void convertCharTOInt(checkersPos* src, int* row, int* col);
void convertIntTOChar(checkersPos* src, int* row, int* col);
//builds tree of possible routes
void buildTree(SingleSourceMovesTreeNode* source, int row, int col, Board board, char player, unsigned short captures);
void checkNext(SingleSourceMovesTreeNode* source, int row, int col, Board board, char* pLeft, char* pRight, char player);
void sortLeftT(SingleSourceMovesTreeNode* leftMove, char* pLeft, int row, int col, Board board, unsigned short captures, char player);
void sortRightT(SingleSourceMovesTreeNode* rightMove, char* pRight, int row, int col, Board board, unsigned short captures, char player);
void sortLeftB(SingleSourceMovesTreeNode* leftMove, char* pLeft, int row, int col, Board board, unsigned short captures, char player);
void sortRightB(SingleSourceMovesTreeNode* leftMove, char* pLeft, int row, int col, Board board, unsigned short captures, char player);
void checkLeft(char* pLeft, int row, int col, Board board, Player player);
void checkRight(char* pRight, int row, int col, Board board, Player player);
void copyBoard(Board newBoard, Board currentBoard);