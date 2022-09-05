#include "Cursor.h"
#include "Screen.h"
#include <string>
using namespace std;

void input(bool& stop, double& movePeriod) {
	if (_kbhit()) {
		char c = _getch();
		if (c == ' ')
			stop = !stop;
		if (c == '1')
			movePeriod = 1000.0 / 1;
		if (c == '2')
			movePeriod = 1000.0 / 2;
		if (c == '3')
			movePeriod = 1000.0 / 3;
		if (c == '4')
			movePeriod = 1000.0 / 4;
		if (c == '5')
			movePeriod = 1000.0 / 5;
	}
}

int main() {
	Screen scr;
	int n, m, setSpeed;
	cin >> n >> m >> setSpeed;

	double movePeriod = 1000.0 / setSpeed;
	double cursorPeriod = 1000.0 / 4;
	bool stop = false;
	vector<vector<int>> map(m, vector<int>(n, 0));

	Cursor cursor(n, m);

	clock_t moveStart = clock();
	clock_t cursorStart = clock();
	while (1)
	{
		clock_t moveCurrent = clock();
		clock_t cursorCurrent = clock();

		input(stop, movePeriod);

		scr.Render(cursor.GetCurrX(), cursor.GetCurrY(),map,cursor.GetBlink() ? "бс" : " ");

		if ((double)moveCurrent - moveStart >= movePeriod && !stop) {
			cursor.Move(map);
			moveStart = moveCurrent;
		}

		if ((double)cursorCurrent - cursorStart >= cursorPeriod) {
			cursor.Blink();
			cursorStart = cursorCurrent;
		}
	}
}