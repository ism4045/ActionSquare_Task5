#include "Player.h"
#include <ctime>
#include <map>
Player::Player()
{
	srand(time(NULL));
	for (int i = 0; i < 5; i++) {
		int temp = rand() % 7;
		nextBlockType.push((BlockType)temp);
		Block blockTemp;
		blockTemp.initialize((BlockType)temp);
		nextBlock.push_back(blockTemp);
	}
	defaultPos = { 1,4 };

	board.resize(21);
	board[20].resize(10, -2);
	for (int i = 0; i < board.size()-1; i++) {
		board[i].resize(10, 8);
	}

	SpawnBlock();
	gameOver = false;
}

void Player::SpawnBlock()
{
	Pos = defaultPos;

	currentBlockType = nextBlockType.front();
	currentBlock.initialize(currentBlockType);

	nextBlockType.pop();
	nextBlock.erase(nextBlock.begin());
	srand(time(NULL));
	int temp = rand() % 7;
	nextBlockType.push((BlockType)temp);
	Block blockTemp;
	blockTemp.initialize((BlockType)temp);
	nextBlock.push_back(blockTemp);

	if (!CanProcess()) Pos.first = 0;
	if (!CanProcess()) gameOver = true;
	UpdatePlayerBlock();
}

void Player::FallBlock()
{
	if (!CanFall()) {
		SpawnBlock();
	}
	else {
		for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
			int tileX = Pos.first + currentBlock.tileInfo[i].offsetX;
			int tileY = Pos.second + currentBlock.tileInfo[i].offsetY;
			if (tileX >= 0) {
				board[tileX][tileY] = 8;
			}
		}
		Pos.first++;
		UpdatePlayerBlock();
	}
}

void Player::UpdatePlayerBlock()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].offsetX;
		int tileY = Pos.second + currentBlock.tileInfo[i].offsetY;
		if (tileX >= 0) {
			board[tileX][tileY] = currentBlockType;
		}
	}
}

bool Player::CanProcess()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].offsetX;
		int tileY = Pos.second + currentBlock.tileInfo[i].offsetY;
		if (tileX >= 0) {
			if (board[tileX][tileY] != 8) {
				return false;
			}
		}
	}
	return true;
}

bool Player::CanFall()
{
	map<int, int> shouldCheckTile;
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].offsetX;
		int tileY = Pos.second + currentBlock.tileInfo[i].offsetY;
		if (shouldCheckTile.find(tileY) != shouldCheckTile.end()) {
			if (shouldCheckTile[tileY] < tileX) shouldCheckTile[tileY] = tileX;
		}
		else shouldCheckTile[tileY] = tileX;
	}

	for (auto iter = shouldCheckTile.begin(); iter != shouldCheckTile.end(); iter++) {
		if (board[(iter->second) + 1][iter->first] != 8) {
			return false;
		}
	}
	return true;
}
