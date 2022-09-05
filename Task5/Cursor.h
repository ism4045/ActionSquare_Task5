#pragma once

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <vector>

using namespace std;

class Cursor {
public:
	Cursor(const int& x, const int& y);

	bool CanMove(const int& x, const int& y, const vector<vector<int>>& map);
	bool EndGame(const vector<vector<int>>& map);
	void ChangeDir();
	void Move(vector<vector<int>>& map);


	void Blink();

	int GetCurrX() { return curr_X; }
	int GetCurrY() { return curr_Y; }

	bool GetBlink() { return isBlink; }
private:
	bool isBlink;

	int n, m;

	int curr_X, curr_Y;

	int dx[4] = { 1,0,-1,0 };
	int dy[4] = { 0,1,0,-1 };
	int curr_Dir;
};