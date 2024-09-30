#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "game.h"

int CapturesOneTurn;
char finalTurn[4];

void main()
{
	//creates board
	char starting_player;
	Board board = {
		{' ', 'T', ' ', 'T', ' ', 'T', ' ', 'T'},
		{'T', ' ', 'T', ' ', 'T', ' ', 'T', ' '},
		{' ', 'T', ' ', 'T', ' ', 'T', ' ', 'T'},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{'B', ' ', 'B', ' ', 'B', ' ', 'B', ' '},
		{' ', 'B', ' ', 'B', ' ', 'B', ' ', 'B'},
		{'B', ' ', 'B', ' ', 'B', ' ', 'B', ' '}
	};

	//asks user to choose starting player
	printf("Please choose a starting player T or B  \n");
	scanf("%c", &starting_player);

	//plays game
	playGame(board, starting_player);
}

//determine possible move and plays it
void Turn(Board board, Player player) {
	//best move is the returned value, player moves is all the possible moves the player has
	SingleSourceMovesList* bestMove = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkAlloc(bestMove, "best move allocation failed");

	MultipleSourceMovesList* playerMoves = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
	checkAlloc(playerMoves, "playerMoves allocation failed");

	MultipleSourceMovesListCell* curr = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell));
	checkAlloc(curr, "curr allocation failed");

	//finds possible moves
	playerMoves = FindAllPossiblePlayerMoves(board, player);
	curr = playerMoves->head;
	while (curr->single_source_moves_list->head->next == NULL) //continues until there is a playable move
	{
		curr = curr->next;
	}
	bestMove = curr->single_source_moves_list;//assigns first possible move to best move
	curr = curr->next;//moves curr to next

	//goes over list of possible moves and finds the best move
	while (curr != NULL)
	{
		//checks to see which route has the most captures
		if (bestMove->tail->captures < curr->single_source_moves_list->tail->captures) {
			bestMove = curr->single_source_moves_list;
		}
		//if num of captures is the same
		else if (bestMove->tail->captures == curr->single_source_moves_list->tail->captures)
		{
			//checks if player is T
			if (player == 'T')
			{
				if (curr->single_source_moves_list->head->next != NULL)//if there is no move
				{
					//checks which row is bigger
					if (curr->single_source_moves_list->head->position->row > bestMove->head->position->row)
					{
						bestMove = curr->single_source_moves_list;
					}
					//if the rows are the same check the cols
					else if (curr->single_source_moves_list->head->position->row == bestMove->head->position->row
						&& curr->single_source_moves_list->head->position->col > bestMove->head->position->col)
					{
						bestMove = curr->single_source_moves_list;
					}
				}
			}
			//checks if the player B
			else
			{
				if (curr->single_source_moves_list->head->next != NULL)//if there is no move
				{
					//checks which row is smaller
					if (curr->single_source_moves_list->head->position->row < bestMove->head->position->row)
					{
						bestMove = curr->single_source_moves_list;
					}
					//if the rows are the same check the cols
					else if (curr->single_source_moves_list->head->position->row == bestMove->head->position->row
						&& curr->single_source_moves_list->head->position->col < bestMove->head->position->col)
					{
						bestMove = curr->single_source_moves_list;
					}
				}
			}
		}
		curr = curr->next;
	}
	//plays the move that was chosen
	playTurn(bestMove, board, player);
}

//plays the turn, updates the board,
void playTurn(SingleSourceMovesList* bestMove, Board board, Player player)
{
	SingleSourceMovesListCell* curr = bestMove->head;
	SingleSourceMovesListCell* next = curr->next;
	int colNext, rowNext;

	//goes over list and updates board
	for (int i = 0; i < bestMove->tail->captures; i++)
	{
		convertCharTOInt(next->position, &rowNext, &colNext);

		//checks if the capture is from the left or from the right, and executes
		if (player == 'T')
		{
			if (curr->position->col < next->position->col)
			{
				board[rowNext - 1][colNext - 1] = ' ';
			}
			else
			{
				board[rowNext - 1][colNext + 1] = ' ';
			}
		}
		else
		{
			if (curr->position->col < next->position->col)
			{
				board[rowNext + 1][colNext - 1] = ' ';
			}
			else
			{
				board[rowNext + 1][colNext + 1] = ' ';
			}
		}
		curr = next;
		next = curr->next;
	}
	//update global var's
	CapturesOneTurn = bestMove->tail->captures;//saves amount of captures
	//saves curr pos to new pos
	finalTurn[0] = bestMove->head->position->row;
	finalTurn[1] = bestMove->head->position->col;
	finalTurn[2] = bestMove->tail->position->row;
	finalTurn[3] = bestMove->tail->position->col;

	//updates player location
	convertCharTOInt(bestMove->head->position, &rowNext, &colNext);
	board[rowNext][colNext] = ' ';
	convertCharTOInt(bestMove->tail->position, &rowNext, &colNext);
	board[rowNext][colNext] = player;
}

//plays game, executes turns, checks if game is over
void playGame(Board board, Player starting_player)
{
	int movesB = 0, movesT = 0, mostCaptures = 0;
	char playerMostCaptures = starting_player;
	//plays turn until game is over
	playGameHelper(board, starting_player, &movesB, &movesT, &mostCaptures, &playerMostCaptures);

	//prints game stats
	printf("%c wins! \n", starting_player);
	if (starting_player == 'B')
	{
		printf("%c performed %d moves. \n", starting_player, movesB);
	}
	else
	{
		printf("%c performed %d moves. \n", starting_player, movesT);
	}
	printf("%c performed the highest number of captures in a single move - %d \n", playerMostCaptures, mostCaptures);
}

//plays game until game is over
void playGameHelper(Board board, Player starting_player, int* movesB, int* movesT, int* mostCaptures, char* playerMostCaptures)
{
	bool noPawns = printBoard(board), gameOver = false;
	//plays game until there is a winner
	while (!gameOver)
	{
		//plays turn
		Turn(board, starting_player);

		//checks if the number of captures in this turn is bigger then the number of captures we had before it
		if (CapturesOneTurn > *mostCaptures)
		{
			*mostCaptures = CapturesOneTurn;
			playerMostCaptures = starting_player;
		}

		//prints turn and players curr pos and new pos
		printf("\n%c's turn: \n", starting_player);
		printf("%c%c->%c%c \n", finalTurn[0], finalTurn[1], finalTurn[2], finalTurn[3]);
		//prints board and checks if there are no more pawns for one of the players
		noPawns = printBoard(board);

		//updates stats and checks if there is a winner
		if (starting_player == 'T')
		{
			(*movesT)++;//adds a move to T
			//check if T won
			if (noPawns || finalTurn[2] == 'H') {
				gameOver = true;
			}
			else
			{
				starting_player = 'B';//next players turn
			}
		}
		else
		{
			(*movesB)++;//adds a move to B
			//check if B won
			if (noPawns || finalTurn[2] == 'A') {
				gameOver = true;
			}
			else
			{
				starting_player = 'T';//next players turn
			}
		}
	}
}

//prints board and checks if a player lost all of his pawns
bool printBoard(Board board) {
	bool noPawnsB = true, noPawnsT = true;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		//prints first rows of the board
		if (i == 0)
		{
			printf("+-+-+-+-+-+-+-+-+-+ \n");
			printf("+ |1|2|3|4|5|6|7|8| \n");
			printf("+-+-+-+-+-+-+-+-+-+ \n");
		}

		for (int j = 0; j <= BOARD_SIZE; j++)
		{
			//prints the rows alphabetical values
			if (j == 0)
			{
				printf("|%c|", 'A' + i);
			}
			else
			{
				//checks if the players pawns are gone
				if (board[i][j - 1] == 'T')
				{
					noPawnsT = false;
				}
				if (board[i][j - 1] == 'B')
				{
					noPawnsB = false;
				}

				printf("%c|", board[i][j - 1]);
			}
		}
		printf("\n");
		printf("+-+-+-+-+-+-+-+-+-+ \n");
	}
	//if one of the players has no pawns left return true
	if (noPawnsB || noPawnsT)
	{
		return true;
	}
	else
	{
		return false;
	}
}