#pragma once
#include <vector>
#include <queue>
#include <Windows.h>
#include "Block.h"
using namespace std;

enum State
{
	playing,
	removing,
	gameover
};
class Player {
public:
	Player();

	void SpawnBlock();
	void FallBlock();
	void MoveLR(char c);
	void RotateBlock();
	void UpdatePlayerBlock();
	void CheckCompleteLine();
	void CalculateInfo(int clearLineNum);

	bool CanProcess();
	bool CanFall();
	bool CanMoveLR(char c);
	bool CanRotate(int& moveX, int& moveY);

	State getState() { return playerState; }
	void setState(State state) { playerState = state; }
	int getLine() { return line; }
	int getLevel() { return level; }
	int getScore() { return score; }

	vector<Block> getNextBlock() { return nextBlock; }
	vector<vector<int>> getBoard() { return board; }

private:
	State playerState;

	queue<BlockType> nextBlockType;
	vector<Block> nextBlock;

	BlockType currentBlockType;
	Block currentBlock;
	int currentRotate;

	pair<int, int> defaultPos;
	pair<int, int> Pos;
	vector<int> removeLine;

	vector<vector<int>> board;

	int line;
	int level;
	int score;
};