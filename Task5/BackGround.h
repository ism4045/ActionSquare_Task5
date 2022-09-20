#pragma once
#include <vector>
#include <string>
using namespace std;
class BackGround {
public:
	BackGround();
	void MakePlayBox();
	void MakeNextBox();
	void MakeManual();
	void MakeIntro();
	void MakeROE();
	void MakeInfo();

	vector<vector<string>> playBox;
	vector<vector<string>> nextBox;

	vector<string> introStr;
	vector<string> manual;
	vector<string> ROEStr;
	vector<string> infoStr;
};