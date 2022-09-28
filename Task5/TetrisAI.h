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
	int weight;

	Destination(pair<int, int> pos, int rotate, int weight)
		:pos(pos),
		rotate(rotate),
		weight(weight)
	{}

	Destination()
		:Destination({ 0,0 }, 0, 0)
	{}

	Destination(Tetris& t)
		:Destination(t.GetPos(), t.GetCurrentRotate(), 0)
	{}
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

	void Evaluate(Tetris t, vector<Destination>& destVec);
	void Choose(vector<Destination> destVect, Destination& dest);


	void FillAIBehaviors();
	void FillRotateBehaviors(Destination dest);
	void FillMoveBehaviors(Destination dest);
	void FillDropBehaviors(Destination dest);


	Destination GetDestination();
	Destination EvaluateWeight(Tetris t);
	int EvaluateHightestHeight(Tetris& t);
	int EvaluateContactSurface(Tetris& t);
	int EvaluateClearLine(Tetris& t);
	int EvaluateBlankSpace(Tetris& t);

	void GotoWannaXPos(Tetris& t, int xpos);
	void EvaluateCurrentXpos(Tetris& t, vector<Destination>& destinations);

	pair<int, int> afterRotatePos(Destination dest);

	Tetris& GetTetris() { return *tetris; }
private:
	Tetris* tetris;

	queue<FUNC_VOID> aiBehaviors;

	clock_t aiStart;
	clock_t aiCurrent;
};

#endif // !TETRISAI_H_
