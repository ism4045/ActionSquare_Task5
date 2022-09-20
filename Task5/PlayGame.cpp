#include "PlayGame.h"

#define ROCK_DELEY 500.0
#define DEFAULTPOS {1,4}
#define FLOOR -2
#define BLANK 8
#define PREVIEWBLOCK 9
PlayGame::PlayGame()
{
	Initialize();
	mciSendString(TEXT("open Move.mp3 type mpegvideo alias move"), NULL, 0, NULL);
	mciSendString(TEXT("open LineClear.mp3 type mpegvideo alias lineclear"), NULL, 0, NULL);

	mciSendString(TEXT("setaudio move volume to 100"), NULL, 0, NULL);
	mciSendString(TEXT("setaudio lineclear volume to 100"), NULL, 0, NULL);
}

void PlayGame::Initialize()
{
	playGameState = Starting;

	while (!RandomBag.empty())
		RandomBag.pop();
	FillBag();
	nextBlock = RandomBag.front();

	Pos = DEFAULTPOS;
	board.resize(21);
	board[20].assign(10, FLOOR);
	for (int i = 0; i < board.size() - 1; i++) {
		board[i].assign(10, BLANK);
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
	gameStart = gameCurrent - (clock_t)gamePeriod;
	stop = false;
}

void PlayGame::UpdatePerFrameGame()
{
	if (playGameState == Starting) {
		SpawnBlock();
		if (playGameState == GameOver) return; 
	}
	else if (playGameState == Falling) {
		RemoveCurrentBlock();
		Pos.first++;
		SetPreviewBlock();
		UpdateCurrentBlock();
	}
}

void PlayGame::UpdateState()
{
	if (!CanFall(Pos)) {
		playGameState = Waiting;
		gamePeriod = ROCK_DELEY;
	}
	else {
		playGameState = Falling;
		gamePeriod = 1000.0 / processSpeed;
	}
}

void PlayGame::Update()
{
	if (!stop) {
		gameCurrent = clock();
		UpdateState();
	}
	if ((double)gameCurrent - (double)gameStart >= gamePeriod && !stop) {
		if (playGameState == Waiting) {
			if (!CanFall(Pos)) {
				CheckCompleteLine();
				levelSpeed = level + 1;
				processSpeed = levelSpeed;
				softDropSpeed = levelSpeed + 10;
				playGameState = Starting;
			}
			else {
				playGameState = Falling;
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
		srand((unsigned int)time(NULL));
		int temp = rand() % BlockTable.size();
		RandomBag.push(Block(BlockTable.at(temp)));
		BlockTable.erase(BlockTable.begin() + temp);
	}
}

void PlayGame::SpawnBlock()
{
	Pos = DEFAULTPOS;
	currentRotate = 0;
	currentBlock = nextBlock;

	RandomBag.pop();
	if (RandomBag.empty()) FillBag();
	nextBlock = RandomBag.front();

	if (!CanProcess()) Pos.first = 0;
	if (!CanProcess()) 
		playGameState = GameOver;
	else {
		SetPreviewBlock();
		UpdateCurrentBlock();
	}

	
}

void PlayGame::MoveL()
{
	if (!stop) {
		if (CanMoveL()) {
			RemoveCurrentBlock();
			Pos.second--;
			SetPreviewBlock();
			UpdateCurrentBlock();
			mciSendString(TEXT("seek move notify to start"), NULL, 0, NULL);
			mciSendString(TEXT("play move"), NULL, 0, NULL);
		}
	}
}

void PlayGame::MoveR()
{
	if (!stop) {
		if (CanMoveR()) {
			RemoveCurrentBlock();
			Pos.second++;
			SetPreviewBlock();
			UpdateCurrentBlock();
			mciSendString(TEXT("seek move notify to start"), NULL, 0, NULL);
			mciSendString(TEXT("play move"), NULL, 0, NULL);
		}
	}
}

void PlayGame::RotateBlock()
{
	int moveX = 0;
	int moveY = 0;
	if (!stop) {
		if (CanRotate(moveX, moveY)) {
			RemoveCurrentBlock();
			currentBlock.rotateBlock();
			currentRotate = currentRotate == 3 ? 0 : currentRotate + 1;
			Pos.first = moveX;
			Pos.second = moveY;
			SetPreviewBlock();
			UpdateCurrentBlock();
			mciSendString(TEXT("seek move notify to start"), NULL, 0, NULL);
			mciSendString(TEXT("play move"), NULL, 0, NULL);
		}
	}
}

void PlayGame::UpdateCurrentBlock()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0) {
			board[tileX][tileY] = currentBlock.Type;
		}
	}
}

void PlayGame::RemoveCurrentBlock()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0) {
			board[tileX][tileY] = BLANK;
		}
	}
}

void PlayGame::CheckCompleteLine()
{
	vector<int> removeLine;
	for (int i = 0; i <board.size()-1; i++) {
		int isComplete = true;
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] == BLANK) {
				isComplete = false;
				break;
			}
		}
		if (isComplete) {
			board[i].assign(10, BLANK);
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
			board[0].assign(10, BLANK);
		}
		removeLine.clear();

		mciSendString(TEXT("seek lineclear notify to start"), NULL, 0, NULL);
		mciSendString(TEXT("play lineclear"), NULL, 0, NULL);
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



void PlayGame::Stop()
{
	clock_t temp = gameCurrent - gameStart;
	if (stop) {
		mciSendString(TEXT("play play notify repeat"), NULL, 0, NULL);
		gameCurrent = clock();
		gameStart = gameCurrent - temp;
	}
	else
		mciSendString(TEXT("stop play"), NULL, 0, NULL);
	stop = !stop;
}

void PlayGame::ChangeSoftDrop()
{
	if (CanFall(Pos)) {
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
	if (!stop) {
		RemoveCurrentBlock();
		while (CanFall(Pos))
			Pos.first++;
		UpdateCurrentBlock();
		gameStart = gameCurrent - (clock_t)ROCK_DELEY;
	}
}

void PlayGame::SetPreviewBlock()
{
	pair<int, int> tempPos = Pos;
	while (CanFall(tempPos))
	{
		tempPos.first++;
	}
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] == PREVIEWBLOCK) board[i][j] = BLANK;
		}
	}
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = tempPos.first + currentBlock.tileInfo[i].X;
		int tileY = tempPos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0) {
			board[tileX][tileY] = PREVIEWBLOCK;
		}
	}
}

bool PlayGame::CanProcess()
{
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		if (tileX >= 0 && tileY >= 0 && tileY < 10) {
			if (board[tileX][tileY] != BLANK && board[tileX][tileY] != PREVIEWBLOCK) {
				return false;
			}
		}
	}
	return true;
}

bool PlayGame::CanFall(pair<int, int> p)
{
	map<int, int> shouldCheckTile;
	for (int i = 0; i < currentBlock.tileInfo.size(); i++) {
		int tileX = p.first + currentBlock.tileInfo[i].X;
		int tileY = p.second + currentBlock.tileInfo[i].Y;
		if (shouldCheckTile.find(tileY) != shouldCheckTile.end()) {
			if (shouldCheckTile[tileY] < tileX) shouldCheckTile[tileY] = tileX;
		}
		else shouldCheckTile[tileY] = tileX;
	}

	for (auto iter = shouldCheckTile.begin(); iter != shouldCheckTile.end(); iter++) {
		if (board[(iter->second) + 1][iter->first] != BLANK && board[(iter->second) + 1][iter->first] != PREVIEWBLOCK) {
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
		else if (board[iter->first][posY] != BLANK && board[iter->first][posY] != PREVIEWBLOCK) {
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
		else if (board[iter->first][posY] != BLANK && board[iter->first][posY] != PREVIEWBLOCK) {
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
			tempBoard[tileX][tileY] = BLANK;
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
			if (tempTileX < 0 || tempTileY < 0 || tempTileX >= 20 || tempTileY >= 10 || (tempBoard[tempTileX][tempTileY] != BLANK && tempBoard[tempTileX][tempTileY] != PREVIEWBLOCK)) {
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
