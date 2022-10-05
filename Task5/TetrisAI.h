#pragma once
#ifndef TETRISAI_H_

#define TETRISAI_H_

#include <functional>
#include <vector>
#include <queue>
#include <ctime>
#include "Tetris.h"

typedef function<void(Tetris*)> FUNC_VOID;

struct Destination
{
	pair<int, int> pos;
	int rotate;
	double weight;

	Destination(pair<int, int> pos, int rotate, double weight)
		:pos(pos),
		rotate(rotate),
		weight(weight)
	{}

	Destination()
		:Destination({ 0,0 }, 0, 0)
	{}

	Destination(Tetris& t)
		:Destination(t.GetPos(), t.GetCurrentRotate(), 0)
	{
		t.DoHardDrop();
		this->pos = t.GetPos();
	}
};

using namespace std;

class TetrisAI
{
public:
	TetrisAI();
	~TetrisAI();


	void Init();
	void Update();
	void PlayAI();

	vector<Destination> Evaluate(Tetris t);
	Destination Choose(vector<Destination> destVect);

	void GotoWannaXPos(Tetris& t, int xpos);
	void EvaluateCurrentXpos(Tetris& t, vector<Destination>& destinations);


	void FillAIBehaviors();
	void FillRotateBehaviors(Destination dest);
	void FillMoveBehaviors(Destination dest);
	void FillDropBehaviors(Destination dest);


	Destination GetDestination();
	Destination EvaluateWeight(Tetris t);
	double EvaluateHightestHeight(Tetris& t);
	double EvaluateContactSurface(Tetris& t);
	double EvaluateClearLine(Tetris& t);
	double EvaluateBlankSpace(Tetris& t);

	
	Tetris& GetTetris() { return *tetris; }
private:
	Tetris* tetris;

	queue<FUNC_VOID> aiBehaviors;

	clock_t aiStart;
	clock_t aiCurrent;

	const double HIGHEST_HEIGHT_NUM = 5.6;
	const double EMPTY_BETWEEN_BLOCKS_NUM = -4.8;
	const double CLEARLINE_NUM = 7.5;
	const double TOUCH_NUM = 4.0;
};

#endif // !TETRISAI_H_
