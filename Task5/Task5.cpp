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

	int levelSpeed = 1;
	int softDropSpeed = levelSpeed + 10;
	int processSpeed = levelSpeed;
	double movePeriod = 1000.0 / 1;
	double cursorPeriod = 1000.0 / 4;
	bool stop = false;
	bool isPause = false;
	clock_t moveStart = clock();
	clock_t moveCurrent;
	clock_t temp;
	while (P.getState() != gameover)
	{
		if (P.getState() != gameover) {
			movePeriod = 1000.0 / processSpeed;
			if (!stop) moveCurrent = clock();
			char command = input();
			if (command == ' ') {
				if (!stop) {
					temp = moveCurrent - moveStart;
				}
				else {
					moveCurrent = clock();
					moveStart = moveCurrent - temp;
				}
				stop = !stop;
			}
			if (command == 77 || command == 75) P.MoveLR(command);
			if (command == 72) P.RotateBlock();
			if (GetAsyncKeyState(VK_DOWN) & 0x8000) processSpeed = softDropSpeed;
			else processSpeed = levelSpeed;

			if (moveCurrent - moveStart >= movePeriod && !stop) {
				P.FallBlock();
				levelSpeed = P.getLevel() + 1;
				softDropSpeed = levelSpeed + 10;
				moveStart = moveCurrent;
			}
		}
		scr.Render();
	}
}