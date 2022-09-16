#include "BackGround.h"

BackGround::BackGround()
{
	makePlayBox();
	makeNextBox();
	makeManual();
}

void BackGround::makePlayBox() {
	PlayBox.resize(22);
	for (int i = 0; i < PlayBox.size(); i++) {
		if (i == 0) {
			PlayBox[i].push_back(" ");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back("¡ã");
			PlayBox[i].push_back(" ");
		}
		else if (i == 21) {
			PlayBox[i].push_back(" ");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back("¡å");
			PlayBox[i].push_back(" ");
		}
		else {
			PlayBox[i].push_back("¢¸");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back(" ");
			PlayBox[i].push_back("¢º");
		}
	}
}

void BackGround::makeNextBox() {
	nextBox.resize(5);
	for (int i = 0; i < nextBox.size(); i++) {
		if (i == 0) {
			nextBox[i].push_back(" ");
			nextBox[i].push_back("N");
			nextBox[i].push_back("E");
			nextBox[i].push_back("X");
			nextBox[i].push_back("T");
			nextBox[i].push_back(" ");
		}
		else if (i == 1) {
			nextBox[i].push_back(" ");
			for (int j = 1; j < 5; j++) 
				nextBox[i].push_back("¡ã");
			nextBox[i].push_back(" ");
		}
		else if (i == 4) {
			nextBox[i].push_back(" ");
			for (int j = 1; j < 5; j++)
				nextBox[i].push_back("¡å");
			nextBox[i].push_back(" ");
		}
		else {
			nextBox[i].push_back("¢¸");
			for (int j = 1; j < 5; j++)
				nextBox[i].push_back(" ");
			nextBox[i].push_back("¢º");
		}
	}
}

void BackGround::makeManual()
{
	manual.push_back("¡ç¡æ : Move Left Right");
	manual.push_back("¡è : Rotate Clock Direction");
	manual.push_back("¡é : Soft Drop");
	manual.push_back("Space : Hard Drop");
}
