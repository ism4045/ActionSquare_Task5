#pragma once

#include <Windows.h>
#include <ctime>
#include <vector>

using namespace std;

class Screen {
public:

	Screen();
	void ScreenFlipping();
	void ScreenClear();

	void DrawPath(const vector<vector<int>>& map);
	void DrawCursor(const int& x, const int& y, const char* str);
	void Render(const int& x, const int& y, 
		const vector<vector<int>>& map, const char* str);
private:
	bool screenIndex;
	HANDLE doubleBuffer[2];
};