#pragma once
#include <vector>
#include <string>
using namespace std;
class BackGround {
public:
	BackGround();
	void makePlayBox();
	void makeNextBox();
	void makeManual();
	void makeIntroBox();

	vector<vector<string>> playBox;
	vector<vector<string>> nextBox;

	vector<string> introStr;
	vector<string> manual;
};