#pragma once
#include <vector>
#include <string>
using namespace std;
class BackGround {
public:
	BackGround();
	void makePlayBox();
	void makeNextBox();
	vector<vector<string>> PlayBox;
	vector<vector<string>> nextBox;
};