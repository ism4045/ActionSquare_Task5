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
	if (!CanProcess()) playerState = gameover;
	UpdatePlayerBlock();

	currentRotate = 0;
}

void Player::FallBlock()
{
	if (!CanFall()) {
		CheckCompleteLine();
		SpawnBlock();
	}
	else {
		for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
			int tileX = Pos.first + currentBlock.tileInfo[i].X;
			int tileY = Pos.second + currentBlock.tileInfo[i].Y;
			if (tileX >= 0) {
				board[tileX][tileY] = 8;
			}
		}
		Pos.first++;
		UpdatePlayerBlock();
	}
}

void Player::MoveLR(char c)
{
	if (CanMoveLR(c)) {
		for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
			int tileX = Pos.first + currentBlock.tileInfo[i].X;
			int tileY = Pos.second + currentBlock.tileInfo[i].Y;
			if (tileX >= 0) {
				board[tileX][tileY] = 8;
			}
		}
		if (c == 77) Pos.second++;
		else Pos.second--;
		UpdatePlayerBlock();
	}
}

void Player::RotateBlock()
{
	int moveX = 0;
	int moveY = 0;
	if (CanRotate(moveX, moveY)) {
		for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
			int tileX = Pos.first + currentBlock.tileInfo[i].X;
			int tileY = Pos.second + currentBlock.tileInfo[i].Y;
			if (tileX >= 0) {
				board[tileX][tileY] = 8;
			}
		}
		currentBlock.rotateBlock();
		currentRotate = currentRotate == 3 ? 0 : currentRotate + 1;
		Pos.first = moveX;
		Pos.second = moveY;
		UpdatePlayerBlock();
	}
}

void Player::UpdatePlayerBlock()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0) {
			board[tileX][tileY] = currentBlockType;
		}
	}
}

void Player::CheckCompleteLine()
{
	for (int i = 0; i <board.size()-1; i++) {
		int isComplete = true;
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] == 8) {
				isComplete = false;
				break;
			}
		}
		if (isComplete) {
			playerState = removing;
			board[i].assign(10, 8);
			removeLine.push_back(i);
		}
	}
	if (!removeLine.empty()) {
		for (int i = 0; i < removeLine.size(); i++) {
			int temp = removeLine[i];
			for (int j = temp; j >= 1; j--) {
				board[j] = board[j-1];
			}
			board[0].assign(10, 8);
		}
	}
	removeLine.clear();
}

bool Player::CanProcess()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0 && tileY >= 0 && tileY < 10) {
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
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
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

bool Player::CanMoveLR(char c)
{
	map<int, int> shouldCheckTile;
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;

		if (shouldCheckTile.find(tileX) != shouldCheckTile.end()) {
			if (c == 75) { 
				if (shouldCheckTile[tileX] > tileY) {
					shouldCheckTile[tileX] = tileY;
				}
			}
			else { 
				if (shouldCheckTile[tileX] < tileY) {
					shouldCheckTile[tileX] = tileY;
				}
			}
		}
		else shouldCheckTile[tileX] = tileY;
	}

	for (auto iter = shouldCheckTile.begin(); iter != shouldCheckTile.end(); iter++) {
		int posY;
		if (c == 75) posY = iter->second - 1;
		else posY = iter->second + 1;

		if (posY < 0 || posY >= 10) return false;
		else if (board[iter->first][posY] != 8) {
			return false;
		}
	}

	return true;
}

bool Player::CanRotate(int& moveX, int& moveY)
{
	Block tempBlock = currentBlock;
	vector<vector<int>> tempBoard = board;
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0) {
			tempBoard[tileX][tileY] = 8;
		}
	}
	tempBlock.rotateBlock();
	vector<pair<int, int>> tempTable1 = currentBlock.offsetData[currentRotate];
	vector<pair<int, int>> tempTable2 = currentRotate == 3 ? currentBlock.offsetData[0] : currentBlock.offsetData[currentRotate+1];
	for (int i = 0; i < tempTable1.size(); i++) {
		int tempX = Pos.first + tempTable1[i].first - tempTable2[i].first;
		int tempY = Pos.second + tempTable1[i].second - tempTable2[i].second;
		bool CanRotate = true;
		for (int j = 0; j < tempBlock.tileInfo.size(); j++) {
			int tempTileX = tempX + tempBlock.tileInfo[j].X;
			int tempTileY = tempY + tempBlock.tileInfo[j].Y;
			if (tempTileX < 0 || tempTileY < 0 || tempTileX >= 20 || tempTileY >= 10 || tempBoard[tempTileX][tempTileY] != 8) {
				CanRotate = false;
				break;
			}
		}
		if (CanRotate) {
			moveX = tempX;
			moveY = tempY;
			return true;
		}
	}
	return false;
}
