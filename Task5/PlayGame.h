#pragma once
#include <vector>
#include <iostream>
#include <conio.h>
#include <map>
#include <queue>
#include "Block.h"
using namespace std;

enum PlayGameState {
	Starting,
	Falling,
	Waiting,
	GameOver
};

class PlayGame{
public:

	PlayGame();
	void Initialize();

	void UpdatePerFrameGame();
	void Update();

	void FillBag();
	void SpawnBlock();

	void MoveL();
	void MoveR();
	void RotateBlock();
	void UpdatePlayerBlock();
	void Stop();
	void ChangeSoftDrop();
	void ReturnNormalSpeed();
	void DoHardDrop();
	void SetPreviewBlock();

	void CheckCompleteLine();
	void CalculateInfo(int clearLineNum);
	void CheckSetWaiting();

	bool CanProcess();
	bool CanFall();
	bool CanMoveL();
	bool CanMoveR();
	bool CanRotate(int& moveX, int& moveY);

	int GetLine() { return line; }
	int GetLevel() { return level; }
	int GetScore() { return score; }
	bool GetStop() { return stop; }
	PlayGameState GetPlayGameState() { return gameState; }
	Block GetNextBlock() { return nextBlock; }
	vector<vector<int>> GetBoard() { return board; }

private:
	vector<vector<int>> board;

	PlayGameState gameState;

	Block nextBlock;
	queue<Block> RandomBag;

	Block currentBlock;
	int currentRotate;

	pair<int, int> defaultPos;
	pair<int, int> Pos;

	int line;
	int level;
	int score;

	int levelSpeed;
	int processSpeed;
	int softDropSpeed;

	bool stop;
	clock_t gameStart;
	clock_t gameCurrent;
	double gamePeriod;

};