#include "TetrisAI.h"

#define AI_PLAY_INTERVAL 200
#define DEFAULTPOS {1,4}
#define BLANK 8

TetrisAI::TetrisAI()
	:tetris(new Tetris()),
	aiCurrent(clock()),
	aiStart(aiCurrent - AI_PLAY_INTERVAL)
{
	Init();
}

TetrisAI::~TetrisAI()
{
	delete tetris;
}

void TetrisAI::Init()
{
	tetris->Initialize();
	tetris->SetAImode(true);
}

void TetrisAI::Update()
{
	aiCurrent = clock();
	if ((double)aiCurrent - (double)aiStart >= AI_PLAY_INTERVAL){
		aiStart = aiCurrent;
		PlayAI();
	}
}

void TetrisAI::PlayAI()
{
	if (aiBehaviors.empty()) {
		FillAIBehaviors();
	}

	FUNC_VOID behaviorToDoNow = aiBehaviors.front();
	behaviorToDoNow(tetris);
	aiBehaviors.pop();
}

void TetrisAI::FillAIBehaviors()
{
	Destination dest = GetDestination();
	
	FillRotateBehaviors(dest);
	FillMoveBehaviors(dest);
	FillDropBehaviors(dest);
}

void TetrisAI::FillRotateBehaviors(Destination dest)
{
	for (int i = 0; i < dest.rotate; i++) {
		aiBehaviors.push(&Tetris::RotateBlock);
	}
}
void TetrisAI::FillMoveBehaviors(Destination dest)
{
	pair<int, int> defaultPos = afterRotatePos(dest);
	for (int i = 0; i < abs(dest.pos.second - defaultPos.second); i++) {
		aiBehaviors.push(dest.pos.second > defaultPos.second ? &Tetris::MoveR : &Tetris::MoveL);
	}
}
void TetrisAI::FillDropBehaviors(Destination dest)
{
	pair<int, int> defaultPos = DEFAULTPOS;
	if (dest.pos.first - defaultPos.first != 0)
		aiBehaviors.push(&Tetris::DoHardDrop);
}

Destination TetrisAI::GetDestination()
{
	vector<Destination> destVec;
	Destination dest;

	Evaluate(*tetris, destVec);
	Choose(destVec, dest);
	
	return dest;
}

void TetrisAI::Evaluate(Tetris t, vector<Destination>& destVec)
{
	for (int i = 0; i < t.GetBoard()[0].size(); i++) {
		GotoWannaXPos(t, i);
		EvaluateCurrentXpos(t, destVec);
	}
}

void TetrisAI::Choose(vector<Destination> destVec, Destination& dest)
{
	int maxWeight = 0;

	for (int i = 0; i < destVec.size(); i++) {
		if (maxWeight < destVec[i].weight) {
			maxWeight = destVec[i].weight;
			dest = destVec[i];
		}
	}
}

void TetrisAI::GotoWannaXPos(Tetris& t, int xpos)
{
	while (t.GetPos().second != xpos)
	{
		int beforeXpos = t.GetPos().second;

		xpos > t.GetPos().second ? t.MoveR() : t.MoveL();

		if (beforeXpos == t.GetPos().second)
			t.RotateBlock();
	}
}

void TetrisAI::EvaluateCurrentXpos(Tetris& t, vector<Destination>& destVec)
{
	for (int i = 0; i < 4; i++) {
		destVec.push_back(EvaluateWeight(t));
		t.RotateBlock();
	}
}

Destination TetrisAI::EvaluateWeight(Tetris t)
{
	t.DoHardDrop();

	Destination dest(t);

	dest.weight += EvaluateHightestHeight(t);
	dest.weight += EvaluateContactSurface(t);
	//dest.weight += EvaluateBlankSpace(t);
	dest.weight += EvaluateClearLine(t);

	return dest;
}

int TetrisAI::EvaluateHightestHeight(Tetris& t)
{ 
	vector<vector<int>> board = t.GetBoard();
	for (int i = 0; i < board.size() - 1; i++){
		for (int j = 0; j < board[i].size(); j++){
			if (board[i][j] != BLANK){
				return i;
			}
		}
	}

	return 0;
}

int TetrisAI::EvaluateContactSurface(Tetris& t)
{
	int blankNum = 0;

	vector<vector<int>> board = t.GetBoard();
	pair<int, int> Pos = t.GetPos();
	Block currentBlock = t.GetBlock();

	vector<pair<int, int>> dir = { {-1,0}, {0,1},{1,0},{0,-1} };

	for (int i = 0; i < 4; i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		for (int j = 0; j < 4; j++) {
			int cursorX = tileX + dir[j].first;
			int cursorY = tileY + dir[j].second;
			if (cursorX < board.size() && cursorY < board[i].size() && cursorX > -1 && cursorY > -1) {
				if (board[cursorX][cursorY] == BLANK) {
					blankNum++;
				}
			}
		}
	}

	return 16 - blankNum;
}

int TetrisAI::EvaluateBlankSpace(Tetris& t)
{
	return 1;
}

int TetrisAI::EvaluateClearLine(Tetris& t)
{
	t.CheckCompleteLine();

	int clearLine = t.GetLine() - tetris->GetLine();
	
	if (clearLine > 2) {
		clearLine * 100;
	}

	return clearLine;
}

pair<int, int> TetrisAI::afterRotatePos(Destination dest)
{
	Tetris t = *tetris;

	for (int i = 0; i < dest.rotate; i++)
		t.RotateBlock();

	return t.GetPos();
}