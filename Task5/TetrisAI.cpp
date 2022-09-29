#include "TetrisAI.h"

#define AI_PLAY_INTERVAL 200
#define DEFAULT_XPOS 4
#define DEFAULT_YPOS 1
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

	aiBehaviors = queue<FUNC_VOID>();
}

void TetrisAI::Update()
{
	if (tetris->GetPlayGameState() == PlayGameState::GameOver) {
		return;
	}
		
	tetris->Update();

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
	Tetris t = *tetris;
	for (int i = 0; i < dest.rotate; i++)
		t.RotateBlock();

	pair<int, int> defaultPos = t.GetPos();
	for (int i = 0; i < abs(dest.pos.second - defaultPos.second); i++) {
		aiBehaviors.push(dest.pos.second > defaultPos.second ? &Tetris::MoveR : &Tetris::MoveL);
	}
}
void TetrisAI::FillDropBehaviors(Destination dest)
{
	if (dest.pos.first - DEFAULT_YPOS != 0)
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
	double maxWeight = INT_MIN;

	for (int i = 0; i < destVec.size(); i++) {
		if (maxWeight < destVec[i].weight) {
			maxWeight = destVec[i].weight;
			dest = destVec[i];
		}
	}
}

void TetrisAI::GotoWannaXPos(Tetris& t, int xpos)
{
	int rotate = 0;
	while (t.GetPos().second != xpos)
	{
		int beforeXpos = t.GetPos().second;

		xpos > t.GetPos().second ? t.MoveR() : t.MoveL();

		if (beforeXpos == t.GetPos().second) {
			t.RotateBlock();
			rotate++;
		}
		if (rotate == 4)
			return;
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
	Destination dest(t);

	dest.weight += EvaluateHightestHeight(t) * HIGHEST_HEIGHT_NUM;
	dest.weight += EvaluateContactSurface(t) * TOUCH_NUM;
	dest.weight += EvaluateBlankSpace(t) * EMPTY_BETWEEN_BLOCKS_NUM;
	dest.weight += EvaluateClearLine(t) * CLEARLINE_NUM;

	return dest;
}

double TetrisAI::EvaluateHightestHeight(Tetris& t)
{ 
	vector<vector<int>> board = t.GetBoard();
	for (int i = 0; i < board.size() - 1; i++){
		for (int j = 0; j < board[i].size(); j++){
			if (board[i][j] != BLANK){
				return (double)i;
			}
		}
	}

	return 0;
}
double TetrisAI::EvaluateContactSurface(Tetris& t)
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

	return (double)(16 - blankNum);
}
double TetrisAI::EvaluateBlankSpace(Tetris& t)
{
	int blankSpaceNum = 0;

	vector<vector<int>> board = t.GetBoard();
	pair<int, int> Pos = t.GetPos();
	Block currentBlock = t.GetBlock();

	for (int i = 0; i < 4; i++) {
		int tileX = Pos.first + currentBlock.tileInfo[i].X;
		int tileY = Pos.second + currentBlock.tileInfo[i].Y;
		for (int j = tileX; j < board.size(); j++) {
			if (board[j][tileY] == BLANK)
				blankSpaceNum++;
		}
	}
	return (double)blankSpaceNum;
}
double TetrisAI::EvaluateClearLine(Tetris& t)
{
	t.CheckCompleteLine();
	int clearLine = t.GetLine() - tetris->GetLine();

	return (double)clearLine;
}