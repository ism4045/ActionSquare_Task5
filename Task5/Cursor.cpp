#include "Cursor.h"

Cursor::Cursor(const int& x, const int& y) : n(x), m(y), curr_X(0), curr_Y(0), curr_Dir(0), isBlink(true) {}

bool Cursor::CanMove(const int& x, const int& y, const vector<vector<int>>& map)
{
	if (y >= m || x >= n || x < 0 || y < 0 || map[y][x] == 1)
		return false;
	return true;
}

bool Cursor::EndGame(const vector<vector<int>>& map) {
	if (!CanMove(curr_X + 1, curr_Y, map) && !CanMove(curr_X - 1, curr_Y, map) && !CanMove(curr_X, curr_Y - 1, map) && !CanMove(curr_X, curr_Y + 1, map))
		return true;
	return false;
}

void Cursor::ChangeDir() {
	curr_Dir++;
	curr_Dir = curr_Dir > 3 ? 0 : curr_Dir;
}

void Cursor::Move(vector<vector<int>>& map) {
	int next_X = curr_X + dx[curr_Dir];
	int next_Y = curr_Y + dy[curr_Dir];

	if (!CanMove(next_X, next_Y, map)) {
		ChangeDir();
		next_X = curr_X + dx[curr_Dir];
		next_Y = curr_Y + dy[curr_Dir];
	}

	if (CanMove(next_X, next_Y, map)) {
		map[curr_Y][curr_X] = 1;
		curr_X = next_X;
		curr_Y = next_Y;
	}
}

void Cursor::Blink()
{
	isBlink = !isBlink;
}
