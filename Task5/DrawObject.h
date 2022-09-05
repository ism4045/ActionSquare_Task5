#pragma once

#include "Screen.h"
#include <vector>
#include <Windows.h>
using namespace std;

class DrawObject {
public:
	void DrawPath(const vector<vector<int>>& map);
	void DrawCursor(const int& x, const int& y, const char* str);
};