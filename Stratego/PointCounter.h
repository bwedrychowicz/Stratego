#pragma once
class PointCounter {
public:

	int points[2] = { 0,0 };
	int boardSize=7;
	int gameBoard[7][7];
	PointCounter(int boardSize, int **gameBoard);
	~PointCounter();

	int *countPoint(int currentPlayer, POINT p);
};

