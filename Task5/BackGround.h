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
	vector<vector<string>> PlayBox;
	vector<vector<string>> nextBox;
	vector<string> manual;
};