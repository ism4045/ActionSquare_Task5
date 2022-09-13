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

	int highSpeed = 20;
	int defaultSpeed = setSpeed;
	double movePeriod = 1000.0 / setSpeed;
	double cursorPeriod = 1000.0 / 4;
	bool stop = false;

	clock_t moveStart = clock();
	clock_t cursorStart = clock();
	clock_t moveCurrent;
	clock_t cursorCurrent;
	while (P.getState() != gameover)
	{
		movePeriod = 1000.0 / setSpeed;
		if (!stop) moveCurrent = clock();
		cursorCurrent = clock();

		char command = input();
		if (command == ' ') stop = !stop;
		if (command == 77 || command == 75) P.MoveLR(command);
		if (command == 72) P.RotateBlock();
		if (GetKeyState(VK_DOWN) & 0x8000) setSpeed = highSpeed;
		else setSpeed = defaultSpeed;

		if (moveCurrent - moveStart >= movePeriod && !stop) {
			P.FallBlock();
			moveStart = moveCurrent;
		}

		scr.Render();
	}
}