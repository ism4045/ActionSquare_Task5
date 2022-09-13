#pragma once
#include <vector>
#include <queue>
#include "Block.h"
using namespace std;

class Player {
public:

	Player();

	void SpawnBlock();
	void FallBlock();
	void MoveLR();
	void RotateBlock();
	void UpdatePlayerBlock();

	bool CanProcess();
	bool CanFall();
	bool CanMoveLR();

	vector<Block> nextBlock;
	vector<vector<int>> board;

private:
	queue<BlockType> nextBlockType;
	BlockType currentBlockType;
	Block currentBlock;
	int currentRotate;
	pair<int, int> defaultPos;
	pair<int, int> Pos;
	bool gameOver;
};