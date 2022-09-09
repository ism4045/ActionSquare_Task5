#include "BackGround.h"

BackGround::BackGround()
{
	PlayBox.resize(22);
	for (int i = 0; i < 22; i++) {
		if (i == 0) {
			PlayBox[i].push_back(" 旨");
			for (int j = 1; j <= 10; j++) 
				PlayBox[i].push_back(" 收");
			PlayBox[i].push_back(" 旬");
		}
		else if (i == 21) {
			PlayBox[i].push_back(" 曲");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back(" 收");
			PlayBox[i].push_back(" 旭");
		}
		else {
			PlayBox[i].push_back(" 早");
			for (int j = 1; j <= 10; j++)
				PlayBox[i].push_back("  ");
			PlayBox[i].push_back(" 早");
		}
	}
}
