#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <windows.h>

#define BOARD_SIZE 3
using namespace std;

class Game {
	enum class Mark {
		player = 'O',
		AI = 'X',
		empty = '-'
	};
	struct Move {
		unsigned int x = 0;
		unsigned int y = 0;
	};

	Mark board[BOARD_SIZE][BOARD_SIZE];
public:
	Game()
	{
		ClearBoard();
	}
	void ClearBoard()
	{
		for (unsigned int x = 0; x < BOARD_SIZE; x++)
		{
			for (unsigned int y = 0; y < BOARD_SIZE; y++)
			{
				board[x][y] = Mark::empty;
			}
		}
	}
	void DrawGrid() {
		system("cls");
		printf("---------------\n");
		for (unsigned int x = 0; x < BOARD_SIZE; ++x)
		{
			for (unsigned int y = 0; y < BOARD_SIZE; y++)
			{
				printf(" %c |", board[x][y]);
			}
			printf("\n---------------\n");
		}
	}
	bool CanStillPlay(Mark board[BOARD_SIZE][BOARD_SIZE])
	{
		for (unsigned int x = 0; x < BOARD_SIZE; x++)
		{
			for (unsigned int y = 0; y < BOARD_SIZE; y++)
			{
				if (board[x][y] == Mark::empty)
				{
					return true;
				}
			}
		}
		return false;
	};


	int CheckVictory(Mark board[BOARD_SIZE][BOARD_SIZE]) {

#pragma region Rows
		for (int row = 0; row < BOARD_SIZE; row++)
		{
			if (board[row][0] == board[row][1] && board[row][1] == board[row][2])
			{
				if (board[row][0] == Mark::AI)
				{
					return +10;
				}
				else if (board[row][0] == Mark::player) {
					return -10;
				}
			}
		}
#pragma endregion

#pragma region Columns
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			if (board[0][column] == board[1][column] && board[1][column] == board[2][column])
			{
				if (board[0][column] == Mark::AI)
				{
					return +10;
				}
				else if (board[0][column] == Mark::player) {
					return -10;
				}
			}
		}
#pragma endregion

#pragma region Diagonals


		if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
		{
			if (board[0][0] == Mark::AI)
			{
				return +10;
			}
			else if (board[0][0] == Mark::player)
			{
				return -10;
			}
		}
		if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
		{
			if (board[0][2] == Mark::AI)
			{
				return +10;
			}
			else if (board[0][2] == Mark::player)
			{
				return -10;
			}
		}
#pragma endregion


		return 0;
	}
	int Minimax(Mark board[BOARD_SIZE][BOARD_SIZE], int depth, bool bIsMaxValue) {
		int score = CheckVictory(board);
		if (score == 10)
		{
			return score;
		}

		if (score == -10)
		{
			return score;
		}

		if (!CanStillPlay(board))
		{
			return 0;
		}

		if (bIsMaxValue)
		{
			int bestValue = -1000;

			for (int i = 0; i < BOARD_SIZE; i++)
			{
				for (int j = 0; j < BOARD_SIZE; j++)
				{
					if (board[i][j] == Mark::empty)
					{
						board[i][j] = Mark::AI;
						int temp = max(bestValue, Minimax(board, depth + 1, !bIsMaxValue));
						bestValue = max(bestValue, temp);
						board[i][j] = Mark::empty;
					}
				}
			}
			return bestValue;

		}
		else {
			int bestValue = 1000;
			for (int i = 0; i < BOARD_SIZE; i++)
			{
				for (int j = 0; j < BOARD_SIZE; j++)
				{
					if (board[i][j] == Mark::empty)
					{
						board[i][j] = Mark::player;
						int temp = min(bestValue, Minimax(board, depth + 1, !bIsMaxValue));
						bestValue = min(bestValue, temp);
						board[i][j] = Mark::empty;
					}
				}
			}
			return bestValue;
		}
	}


	Move GetCPUMove(Mark board[BOARD_SIZE][BOARD_SIZE])
	{
		int bestValue = -1000;
		Move bestPossibleMove;
		bestPossibleMove.x = -1;
		bestPossibleMove.y = -1;

		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				if (board[i][j] == Mark::empty)
				{
					board[i][j] = Mark::AI;
					int temp = Minimax(board, 0, false);

					board[i][j] = Mark::empty;
					if (temp > bestValue)
					{
						bestPossibleMove.x = i;
						bestPossibleMove.y = j;
						bestValue = temp;
					}
				}
			}
		}
		printf("AI move value is : %d \n", bestValue);
		return bestPossibleMove;
	}
	void GetPlayerMove() {
		bool failedMove = true;
		unsigned int x = -1, y = -1;

		do
		{
			puts("Player Move: ");
			char move;
			std::cin >> move;
			x = move - '0';
			std::cin >> move;
			y = move - '0';

			failedMove = board[x][y] != Mark::empty;

			std::cin.clear();
		} while (failedMove);

		board[x][y] = Mark::player;
	}
	void PlayGame() {
		unsigned int turn = 0;
		bool endGame = false;

		DrawGrid();
		puts("Enter the ROW(0-2) and COLUMN(0-2) you wish to play in: ");

		do
		{
			if (CanStillPlay(board))
			{

				if (turn % 2 == 0)
				{
					GetPlayerMove();

					if (CheckVictory(board))
					{
						puts("Player wins! ");
						endGame = true;
					}
				}
				else
				{
					puts("CPU move: ");
					Move m = GetCPUMove(board);
					board[m.x][m.y] = Mark::AI;
					if (CheckVictory(board))
					{
						puts("AI wins!");
						endGame = true;
					}
				}
				DrawGrid();
				turn++;
			}
			else
			{
				puts("Draw!");
				endGame = true;
			}
		} while (!endGame);
	}

};
int main()
{
	Game game;
	game.PlayGame();
	cin.ignore();
}

