#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "game.h"
//debug
void printTree(SingleSourceMovesTree* tree);
void printTreeHelper(SingleSourceMovesTreeNode* root);
//task 2

SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree) {
	SingleSourceMovesList* lst = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkAlloc(lst, "lst allocation failed");
	makeEmptyList(lst);
	optimalMoveHelper(moves_tree->source, lst); //arranges optimal route
	//inserts first node to list
	insertDataToStartList(lst, moves_tree->source->total_captures_so_far, moves_tree->source->pos->col, moves_tree->source->pos->row);
	return lst;
}

int optimalMoveHelper(SingleSourceMovesTreeNode* source, SingleSourceMovesList* lst) {
	SingleSourceMovesTreeNode* left = source->next_move[0];
	SingleSourceMovesTreeNode* right = source->next_move[1];
	int maxLeft = -1, maxRight = -1;
	if (left == NULL && right == NULL) {
		return source->total_captures_so_far;
	}
	if (left != NULL)
	{
		maxLeft = optimalMoveHelper(left, lst);
	}
	//LRD
	if (right != NULL)
	{
		maxRight = optimalMoveHelper(right, lst);
	}
	//checks if total captures on right is bigger than left
	if (maxLeft > maxRight) {
		insertDataToStartList(lst, left->total_captures_so_far, left->pos->col, left->pos->row);
		return maxLeft;
	}
	else
	{
		insertDataToStartList(lst, right->total_captures_so_far, right->pos->col, right->pos->row);
		return maxRight;
	}
}

void checkAlloc(void* pointer, char txt[]) {
	if (pointer == NULL) {
		printf("%s", txt);
		exit(1);
	}
}

void makeEmptyList(SingleSourceMovesList* lst) {
	lst->head = NULL;
	lst->tail = NULL;
}

void insertDataToStartList(SingleSourceMovesList* lst, int captures, char col, char row) {
	SingleSourceMovesListCell* newHead;
	newHead = createNewListNode(captures, NULL, col, row);
	insertNodeToStartList(lst, newHead);
}

SingleSourceMovesListCell* createNewListNode(int captures, SingleSourceMovesListCell* next, char col, char row) {
	SingleSourceMovesListCell* res;
	res = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
	res->position = (checkersPos*)malloc(sizeof(checkersPos));
	checkAlloc(res->position, "position allocation failed");
	res->position->col = col;
	res->position->row = row;
	res->captures = captures;
	res->next = next;
	return res;
}

void insertNodeToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head) {
	if (isEmptyList(lst) == true) {
		head->next = NULL;
		lst->head = lst->tail = head;
	}
	else {
		head->next = lst->head;
		lst->head = head;
	}
}

bool isEmptyList(SingleSourceMovesList* lst) {
	if (lst->head == NULL) {
		return true;
	}
	else {
		return false;
	}
}

//task 3

MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player) {
	int i, j;
	SingleSourceMovesTree* tree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	checkAlloc(tree, "tree allocation failed");
	MultipleSourceMovesList* playerMoves = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
	checkAlloc(playerMoves, "playerMoves allocation failed");
	makeEmptyListMulti(playerMoves);
	//goes over the board and adds all of the players pawns's moves to a list
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == player)
			{
				checkersPos* src = (checkersPos*)malloc(sizeof(checkersPos));
				//cheack alloc
				convertIntTOChar(src, i, j);
				tree = FindSingleSourceMoves(board, src);//finds the pawns possible moves

				printTree(tree);//for debug
				SingleSourceMovesList* optimalLst = FindSingleSourceOptimalMove(tree);
				insertDataToEndListMulti(playerMoves, optimalLst);//adds best move to list
			}
		}
	}
	return playerMoves;
}
// problem here down
void makeEmptyListMulti(MultipleSourceMovesList* lst) {
	lst->head = NULL;
	lst->tail = NULL;
}

void insertDataToEndListMulti(MultipleSourceMovesList* lst, SingleSourceMovesList* data) {
	MultipleSourceMovesListCell* newTail;
	newTail = createNewListNodeMulti(data, NULL);
	insertNodeToEndListMulti(lst, newTail);
}

MultipleSourceMovesListCell* createNewListNodeMulti(SingleSourceMovesList* data, MultipleSourceMovesListCell* next) {
	MultipleSourceMovesListCell* res;
	res = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell));
	checkAlloc(res, "res allocation failed");
	res->single_source_moves_list = data;
	res->next = next;
	return res;
}

void insertNodeToEndListMulti(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* tail) {
	if (isEmptyListMulti(lst) == true) {
		(lst)->head = (lst)->tail = tail;
	}
	else {
		(lst)->tail->next = tail;
		(lst)->tail = tail;
	}
	tail->next = NULL;
}

bool isEmptyListMulti(MultipleSourceMovesList* lst) {
	if (lst->head == NULL) {
		return true;
	}
	else {
		return false;
	}
}

//for debugging
void printTree(SingleSourceMovesTree* tree) {
	printTreeHelper(tree->source);
	printf("\n");
}

void printTreeHelper(SingleSourceMovesTreeNode* root) {
	if (root == NULL)
		return;

	printTreeHelper(root->next_move[0]);
	printf(" %c%c ", root->pos->row, root->pos->col);
	printTreeHelper(root->next_move[1]);
}