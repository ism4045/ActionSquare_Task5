#include "Screen.h"
#include "BackGround.h"
#include "Player.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>
using namespace std;

char input() {
	if (_kbhit()) {
		char c = _getch();
		return c;
	}
	return NULL;
}

int main() {
	Player P;
	Screen scr(P);
	int setSpeed;
	cin >> setSpeed;

	double movePeriod = 1000.0 / setSpeed;
	double cursorPeriod = 1000.0 / 4;
	bool stop = false;

	clock_t moveStart = clock();
	clock_t cursorStart = clock();
	clock_t moveCurrent;
	clock_t cursorCurrent;

	while (1)
	{
		movePeriod = 1000.0 / setSpeed;
		if (!stop) moveCurrent = clock();
		cursorCurrent = clock();

		char command = input();
		if (command == ' ') stop = !stop;

		if (moveCurrent - moveStart >= movePeriod && !stop) {
			P.FallBlock();
			moveStart = moveCurrent;
		}

		scr.Render();
	}
}