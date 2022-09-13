#include "BackGround.h"

BackGround::BackGround()
{
	makePlayBox();
	makeNextBox();
}

void BackGround::makePlayBox() {
	PlayBox.resize(22);
	for (int i = 0; i < PlayBox.size(); i++) {
		if (i == 0) {
			PlayBox[i].push_back("��");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back("��");
			PlayBox[i].push_back("��");
		}
		else if (i == 21) {
			PlayBox[i].push_back("��");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back("��");
			PlayBox[i].push_back("��");
		}
		else {
			PlayBox[i].push_back("��");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back(" ");
			PlayBox[i].push_back("��");
		}
	}
}

void BackGround::makeNextBox() {
	nextBox.resize(17);
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
			nextBox[i].push_back("��");
			for (int j = 1; j < 5; j++) 
				nextBox[i].push_back("��");
			nextBox[i].push_back("��");
		}
		else if (i == 16) {
			nextBox[i].push_back("��");
			for (int j = 1; j < 5; j++)
				nextBox[i].push_back("��");
			nextBox[i].push_back("��");
		}
		else if (i % 3 == 1) {
			nextBox[i].push_back("��");
			for (int j = 1; j < 5; j++) 
				nextBox[i].push_back("��");
			nextBox[i].push_back("��");
		}
		else {
			nextBox[i].push_back("��");
			for (int j = 1; j < 5; j++)
				nextBox[i].push_back(" ");
			nextBox[i].push_back("��");
		}
	}
}