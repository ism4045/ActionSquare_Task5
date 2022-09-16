#pragma once
#include <vector>
using namespace std;

enum BlockType {
	Z1 = 0,
	Z2 = 1,
	L1 = 2,
	L2 = 3,
	T = 4,
	I = 5,
	O = 6
};

struct Tile
{
	int X;
	int Y;
};

class Block {
public:
	Block();
	Block(BlockType blockType);
	void initialize(BlockType blockType);

	void setTileInfo(BlockType blockType);
	void setTileOffset(BlockType blockType);
	void rotateBlock();

	vector<vector<pair<int, int>>> offsetData;
	vector<Tile> tileInfo;
	BlockType Type;
};