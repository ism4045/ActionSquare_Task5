#include "BackGround.h"

BackGround::BackGround()
{
	PlayBox.resize(22);
	for (int i = 0; i < 22; i++) {
		if (i == 0) {
			PlayBox[i].push_back(" ��");
			for (int j = 1; j <= 10; j++) 
				PlayBox[i].push_back(" ��");
			PlayBox[i].push_back(" ��");
		}
		else if (i == 21) {
			PlayBox[i].push_back(" ��");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back(" ��");
			PlayBox[i].push_back(" ��");
		}
		else {
			PlayBox[i].push_back(" ��");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back("  ");
			PlayBox[i].push_back(" ��");
		}
	}
}
