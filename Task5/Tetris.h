#pragma once
#include "Block.h"
#include <vector>
#include <iostream>
#include <conio.h>
#include <map>
#include <queue>
#include <ctime>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

enum PlayGameState {
	Starting,
	Falling,
	Waiting,
	GameOver
};

class Tetris {
public:

	Tetris();
	void Initialize();

	void UpdatePerFrameGame();
	void UpdateState();
	void Update();

	void FillBag();
	void SpawnBlock();

	void MoveL();
	void MoveR();
	void RotateBlock();
	void Stop();
	void ChangeSoftDrop();
	void ReturnNormalSpeed();
	void DoHardDrop();

	void UpdateCurrentBlock();
	void RemoveCurrentBlock();
	void SetPreviewBlock();

	void CheckCompleteLine();
	void CalculateInfo(int clearLineNum);

	bool CanProcess();
	bool CanFall(pair<int, int> p);
	bool CanMoveL();
	bool CanMoveR();
	bool CanRotate(int& moveX, int& moveY);

	int GetLine() { return line; }
	int GetLevel() { return level; }
	int GetScore() { return score; }
	bool GetStop() { return stop; }
	pair<int, int> GetPos() { return Pos; }
	int GetCurrentRotate() { return currentRotate; }
	PlayGameState GetPlayGameState() { return playGameState; }
	Block GetBlock() { return currentBlock; }
	Block GetNextBlock() { return nextBlock; }
	vector<vector<int>> GetBoard() { return board; }

private:
	vector<vector<int>> board;

	PlayGameState playGameState;

	Block currentBlock;
	Block nextBlock;
	queue<Block> RandomBag;

	int currentRotate;
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