#pragma once
#include <vector>
#include <queue>
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

	bool CanProcess();
	bool CanFall();
	bool CanMoveLR(char c);
	bool CanRotate(int& moveX, int& moveY);

	State getState() { return playerState; }
	void setState(State state) { playerState = state; }

	vector<Block> nextBlock;
	vector<vector<int>> board;

private:
	State playerState;

	queue<BlockType> nextBlockType;
	BlockType currentBlockType;
	Block currentBlock;
	int currentRotate;

	pair<int, int> defaultPos;
	pair<int, int> Pos;
	vector<int> removeLine;
};