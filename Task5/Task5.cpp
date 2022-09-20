#include "Screen.h"
#include "PlayGame.h"
#include "Input.h"
#include "GameManager.h"
using namespace std;

int main() {
	GameManager GM;
	Screen SCR(GM);
	InputManager I(GM);
	
	while (GM.GetGameState() != GameState::End)
	{
		GM.GameUpdate();
		SCR.Render();
		if (_kbhit())
			I.Input();
	}
}