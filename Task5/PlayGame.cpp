#include "PlayGame.h"
#include <ctime>
#include <map>
PlayGame::PlayGame()
{
	Initialize();
}

void PlayGame::Initialize()
{
	gameState = Starting;

	FillBag();
	nextBlock = RandomBag.front();

	defaultPos = { 1,4 };

	board.resize(21);
	board[20].resize(10, -2);
	for (int i = 0; i < board.size() - 1; i++) {
		board[i].resize(10, 8);
	}

	line = 0;
	level = 0;
	score = 0;
	currentRotate = 0;

	levelSpeed = 1;
	softDropSpeed = levelSpeed + 10;
	processSpeed = levelSpeed;
	gamePeriod = 1000.0 / processSpeed;

	gameCurrent = clock();
	gameStart = gameCurrent - gamePeriod;
}

void PlayGame::UpdatePerFrameGame()
{
	if (gameState == Starting) {
		SpawnBlock();
		if (gameState == GameOver) return; 

		CheckSetWaiting();
		if (gameState == Waiting) return;
		else
			gameState = Falling;
		gamePeriod = 1000.0 / processSpeed;
	}
	else if (gameState == Falling) {
		CheckSetWaiting();
		if (gameState == Waiting) return;
		gamePeriod = 1000.0 / processSpeed;
		for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
			int tileX = Pos.first + currentBlock.tileInfo[i].X;
			int tileY = Pos.second + currentBlock.tileInfo[i].Y;
			if (tileX >= 0) {
				board[tileX][tileY] = 8;
			}
		}
		Pos.first++;
		SetPreviewBlock();
		UpdatePlayerBlock();
		CheckSetWaiting();
	}
}

void PlayGame::Update()
{
	if (!stop) gameCurrent = clock();
	if ((double)gameCurrent - (double)gameStart >= gamePeriod && !stop) {
		if (gameState == Waiting) {
			if (!CanFall()) {
				CheckCompleteLine();
				levelSpeed = level + 1;
				processSpeed = levelSpeed;
				softDropSpeed = levelSpeed + 10;
				gameState = Starting;
			}
			else {
				gameState = Falling;
			}
		}
		UpdatePerFrameGame();
		gameStart = gameCurrent;
	}
}

void PlayGame::FillBag()
{
	vector<BlockType> BlockTable = { Z1,Z2,L1,L2,O,T,I };
	while (!BlockTable.empty()) {
		srand(time(NULL));
		int temp = rand() % BlockTable.size();
		RandomBag.push(Block(BlockTable.at(temp)));
		BlockTable.erase(BlockTable.begin() + temp);
	}
}

void PlayGame::SpawnBlock()
{
	Pos = defaultPos;
	currentRotate = 0;
	currentBlock = nextBlock;

	RandomBag.pop();
	if (RandomBag.empty()) FillBag();
	nextBlock = RandomBag.front();

	if (!CanProcess()) Pos.first = 0;
	if (!CanProcess()) gameState = GameOver;
	else {
		SetPreviewBlock();
		UpdatePlayerBlock();
	}

	
}

void PlayGame::MoveL()
{
	if (gameState != Starting) {
		if (CanMoveL()) {
			for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
				int tileX = Pos.first + currentBlock.tileInfo[i].X;
				int tileY = Pos.second + currentBlock.tileInfo[i].Y;
				if (tileX >= 0) {
					board[tileX][tileY] = 8;
				}
			}
			Pos.second--;
			SetPreviewBlock();
			UpdatePlayerBlock();
			CheckSetWaiting();
		}
	}
}

void PlayGame::MoveR()
{
	if (gameState != Starting) {
		if (CanMoveR()) {
			for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
				int tileX = Pos.first + currentBlock.tileInfo[i].X;
				int tileY = Pos.second + currentBlock.tileInfo[i].Y;
				if (tileX >= 0) {
					board[tileX][tileY] = 8;
				}
			}
			Pos.second++;
			SetPreviewBlock();
			UpdatePlayerBlock();
			CheckSetWaiting();
		}
	}
}

void PlayGame::RotateBlock()
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
		SetPreviewBlock();
		UpdatePlayerBlock();
		CheckSetWaiting();
	}
}

void PlayGame::UpdatePlayerBlock()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0) {
			board[tileX][tileY] = currentBlock.Type;
		}
	}
}

void PlayGame::CheckCompleteLine()
{
	vector<int> removeLine;
	for (int i = 0; i <board.size()-1; i++) {
		int isComplete = true;
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] == 8) {
				isComplete = false;
				break;
			}
		}
		if (isComplete) {
			board[i].assign(10, 8);
			removeLine.push_back(i);
		}
	}
	if (!removeLine.empty()) {

		CalculateInfo((int)removeLine.size());

		for (int i = 0; i < removeLine.size(); i++) {
			int temp = removeLine[i];
			for (int j = temp; j >= 1; j--) {
				board[j] = board[j-1];
			}
			board[0].assign(10, 8);
		}
		removeLine.clear();
	}
}

void PlayGame::CalculateInfo(int clearLineNum) {
	int temp = 40;
	if (clearLineNum == 2) temp = 100;
	else if (clearLineNum == 3) temp = 300;
	else if (clearLineNum >= 4) temp = 1200;
	score += temp * (level + 1);
	line += clearLineNum;
	level = line / 10;
}

void PlayGame::CheckSetWaiting()
{
	if (!CanFall()) {
		gameState = Waiting;
		gamePeriod = 500.0;
	}
	else gameState = Falling;
}

void PlayGame::Stop()
{
	clock_t temp = gameCurrent - gameStart;
	if (stop) {
		gameCurrent = clock();
		gameStart = gameCurrent - temp;
	}
	stop = !stop;
}

void PlayGame::ChangeSoftDrop()
{
	if (CanFall()) {
		processSpeed = softDropSpeed;
		gamePeriod = 1000.0 / processSpeed;
	}
}

void PlayGame::ReturnNormalSpeed()
{
	processSpeed = levelSpeed;
	gamePeriod = 1000.0 / processSpeed;
}

void PlayGame::DoHardDrop()
{
	if (gameState != GameOver) {
		while (CanFall())
		{
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
		CheckCompleteLine();
		levelSpeed = level + 1;
		softDropSpeed = levelSpeed + 10;
		processSpeed = levelSpeed;
		gameState = Waiting;
		gamePeriod = 0.0;
	}
}

void PlayGame::SetPreviewBlock()
{
	pair<int, int> tempPos = Pos;
	while (1)
	{
		map<int, int> shouldCheckTile;
		for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
			int tileX = tempPos.first + currentBlock.tileInfo[i].X;
			int tileY = tempPos.second + currentBlock.tileInfo[i].Y;
			if (shouldCheckTile.find(tileY) != shouldCheckTile.end()) {
				if (shouldCheckTile[tileY] < tileX) shouldCheckTile[tileY] = tileX;
			}
			else shouldCheckTile[tileY] = tileX;
		}
		bool canFall = true;
		for (auto iter = shouldCheckTile.begin(); iter != shouldCheckTile.end(); iter++) {
			if (board[(iter->second) + 1][iter->first] != 8 && board[(iter->second) + 1][iter->first] != 9) {
				canFall = false;
				break;
			}
		}
		if (!canFall) break;
		tempPos.first++;
	}
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] == 9) board[i][j] = 8;
		}
	}
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = tempPos.first + currentBlock.tileInfo[i].X;
		int tileY = tempPos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0) {
			board[tileX][tileY] = 9;
		}
	}
}

bool PlayGame::CanProcess()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0 && tileY >= 0 && tileY < 10) {
			if (board[tileX][tileY] != 8 && board[tileX][tileY] != 9) {
				return false;
			}
		}
	}
	return true;
}

bool PlayGame::CanFall()
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
		if (board[(iter->second) + 1][iter->first] != 8 && board[(iter->second) + 1][iter->first] != 9) {
			return false;
		}
	}
	return true;
}

bool PlayGame::CanMoveL()
{
	map<int, int> shouldCheckTile;
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;

		if (shouldCheckTile.find(tileX) != shouldCheckTile.end()) {
			if (shouldCheckTile[tileX] > tileY) {
				shouldCheckTile[tileX] = tileY;
			}
		}
		else shouldCheckTile[tileX] = tileY;
	}

	for (auto iter = shouldCheckTile.begin(); iter != shouldCheckTile.end(); iter++) {
		int posY;
		posY = iter->second - 1;
		if (iter->first < 0 || posY < 0 || posY >= 10) return false;
		else if (board[iter->first][posY] != 8 && board[iter->first][posY] != 9) {
			return false;
		}
	}
	return true;
}

bool PlayGame::CanMoveR()
{
	map<int, int> shouldCheckTile;
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;

		if (shouldCheckTile.find(tileX) != shouldCheckTile.end()) {
			if (shouldCheckTile[tileX] < tileY) {
				shouldCheckTile[tileX] = tileY;
			}
		}
		else shouldCheckTile[tileX] = tileY;
	}

	for (auto iter = shouldCheckTile.begin(); iter != shouldCheckTile.end(); iter++) {
		int posY;
		posY = iter->second + 1;
		if (iter->first < 0 || posY < 0 || posY >= 10) return false;
		else if (board[iter->first][posY] != 8 && board[iter->first][posY] != 9) {
			return false;
		}
	}
	return true;
}

bool PlayGame::CanRotate(int& moveX, int& moveY)
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
			if (tempTileX < 0 || tempTileY < 0 || tempTileX >= 20 || tempTileY >= 10 || (tempBoard[tempTileX][tempTileY] != 8 && tempBoard[tempTileX][tempTileY] != 9)) {
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
