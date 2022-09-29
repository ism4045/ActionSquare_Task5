#include "BackGround.h"

BackGround::BackGround()
{
	MakePlayBox();
	MakeNextBox();
	MakeManual();
	MakeIntro();
	MakeROE();
	MakeInfo();
}

void BackGround::MakePlayBox() {
	playBox.resize(22);
	for (int i = 0; i < playBox.size(); i++) {
		if (i == 0) {
			playBox[i].push_back(" ");
			for (int j = 1; j <= 10; j++)
				playBox[i].push_back("¡ã");
			playBox[i].push_back(" ");
		}
		else if (i == 21) {
			playBox[i].push_back(" ");
			for (int j = 1; j <= 10; j++)
				playBox[i].push_back("¡å");
			playBox[i].push_back(" ");
		}
		else {
			playBox[i].push_back("¢¸");
			for (int j = 1; j <= 10; j++)
				playBox[i].push_back(" ");
			playBox[i].push_back("¢º");
		}
	}
}

void BackGround::MakeNextBox() {
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

void BackGround::MakeManual()
{
	manual.push_back("¡ç¡æ : Move Left Right     ");
	manual.push_back("                           ");
	manual.push_back("¡è : Rotate Clock Direction");
	manual.push_back("                           ");
	manual.push_back("¡é : Soft Drop             ");
	manual.push_back("                           ");
	manual.push_back("Space : Hard Drop          ");
	manual.push_back("                           ");
	manual.push_back("Esc : Pause                ");
}

void BackGround::MakeIntro()
{
	introStr.push_back("             T E T R I S              ");
	introStr.push_back("                                      ");
	introStr.push_back("    Press Enter key to play Tetris    ");
	introStr.push_back("                                      ");
	introStr.push_back("                                      ");
	introStr.push_back("      Developer : Lee Ho Seoung       ");
	introStr.push_back("    AIDeveloper : Oh Tae Sung         ");
}

void BackGround::MakeROE()
{
	ROEStr.push_back("      Game Over     ");
	ROEStr.push_back("                    ");
	ROEStr.push_back("> Restart       End ");
	ROEStr.push_back("  Restart     > End ");
	ROEStr.push_back("                    ");
	ROEStr.push_back("                    ");
	ROEStr.push_back("    WINNER is YOU   ");
	ROEStr.push_back("    WINNER is AI    ");
	ROEStr.push_back("                    ");
	ROEStr.push_back("                    ");
}

void BackGround::MakeInfo()
{
	infoStr.push_back("Level : ");
	infoStr.push_back("        ");
	infoStr.push_back("Line  : ");
	infoStr.push_back("        ");
	infoStr.push_back("Score : ");
}
