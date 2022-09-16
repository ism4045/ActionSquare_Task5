#include "BackGround.h"

BackGround::BackGround()
{
	makePlayBox();
	makeNextBox();
	makeManual();
	makeIntroBox();
}

void BackGround::makePlayBox() {
	playBox.resize(22);
	for (int i = 0; i < playBox.size(); i++) {
		if (i == 0) {
			playBox[i].push_back(" ");
			for (int j = 1; j <= 10; j++)
				playBox[i].push_back("▲");
			playBox[i].push_back(" ");
		}
		else if (i == 21) {
			playBox[i].push_back(" ");
			for (int j = 1; j <= 10; j++)
				playBox[i].push_back("▼");
			playBox[i].push_back(" ");
		}
		else {
			playBox[i].push_back("◀");
			for (int j = 1; j <= 10; j++)
				playBox[i].push_back(" ");
			playBox[i].push_back("▶");
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
				nextBox[i].push_back("▲");
			nextBox[i].push_back(" ");
		}
		else if (i == 4) {
			nextBox[i].push_back(" ");
			for (int j = 1; j < 5; j++)
				nextBox[i].push_back("▼");
			nextBox[i].push_back(" ");
		}
		else {
			nextBox[i].push_back("◀");
			for (int j = 1; j < 5; j++)
				nextBox[i].push_back(" ");
			nextBox[i].push_back("▶");
		}
	}
}

void BackGround::makeManual()
{
	manual.push_back("←→ : Move Left Right");
	manual.push_back("↑ : Rotate Clock Direction");
	manual.push_back("↓ : Soft Drop");
	manual.push_back("Space : Hard Drop");
	manual.push_back("Esc : Pause");
}

void BackGround::makeIntroBox()
{
	introStr.push_back("          이호성의 테트리스           ");
	introStr.push_back("                                      ");
	introStr.push_back("    Press Enter key to play Tetris    ");
	introStr.push_back("                                      ");
	introStr.push_back("                                      ");
	introStr.push_back("    UI Developer    : Lee Ho Seoung   ");
	introStr.push_back("    Logic Developer : Lee Ho Seoung   ");
	introStr.push_back("    Input Developer : Lee Ho Seoung   ");
	introStr.push_back("    Director        : Lee Ho Seoung   ");
}
