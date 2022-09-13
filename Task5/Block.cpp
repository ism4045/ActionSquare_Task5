#include "Block.h"

void Block::initialize(BlockType blockType)
{
	setTileInfo(blockType);
	setTileOffset(blockType);
	Type = blockType;
}

void Block::setTileInfo(BlockType blockType)
{
	tileInfo.clear();
	Tile temp = { 0,0 };
	tileInfo.push_back(temp);
	switch (blockType)
	{
	case 0:
		temp = { -1,0 };
		tileInfo.push_back(temp);
		temp = { -1,-1 };
		tileInfo.push_back(temp);
		temp = { 0,1 };
		tileInfo.push_back(temp);
		break;
	case 1:
		temp = { -1,0 };
		tileInfo.push_back(temp);
		temp = { -1,1 };
		tileInfo.push_back(temp);
		temp = { 0,-1 };
		tileInfo.push_back(temp);
		break;
	case 2:
		temp = { 0,-1 };
		tileInfo.push_back(temp);
		temp = { 0,1 };
		tileInfo.push_back(temp);
		temp = { -1,1 };
		tileInfo.push_back(temp);
		break;
	case 3:
		temp = { 0,-1 };
		tileInfo.push_back(temp);
		temp = { 0,1 };
		tileInfo.push_back(temp);
		temp = { -1,-1 };
		tileInfo.push_back(temp);
		break;
	case 4:
		temp = { -1,0 };
		tileInfo.push_back(temp);
		temp = { 0,-1 };
		tileInfo.push_back(temp);
		temp = { 0,1 };
		tileInfo.push_back(temp);
		break;
	case 5:
		temp = { 0,-1 };
		tileInfo.push_back(temp);
		temp = { 0,1 };
		tileInfo.push_back(temp);
		temp = { 0,2 };
		tileInfo.push_back(temp);
		break;
	case 6:
		temp = { -1,0 };
		tileInfo.push_back(temp);
		temp = { 0,1 };
		tileInfo.push_back(temp);
		temp = { -1,1 };
		tileInfo.push_back(temp);
		break;
	default:
		break;
	}
}

void Block::setTileOffset(BlockType blockType)
{
	offsetData.clear();
	offsetData.resize(4);
	if (blockType < 5) {
		for (int i = 0; i < 5; i++) offsetData[0].push_back({ 0,0 });
		offsetData[1].push_back({ 0,0 });
		offsetData[1].push_back({ 0,1 });
		offsetData[1].push_back({ 1,1 });
		offsetData[1].push_back({ -2,0 });
		offsetData[1].push_back({ -2,1 });
		for (int i = 0; i < 5; i++) offsetData[2].push_back({ 0,0 });
		offsetData[3].push_back({ 0,0 });
		offsetData[3].push_back({ 0,-1 });
		offsetData[3].push_back({ 1,-1 });
		offsetData[3].push_back({ -2,0 });
		offsetData[3].push_back({ -2,-1 });
	}
	else if (blockType == 5) {
		offsetData[0].push_back({ 0,0 });
		offsetData[0].push_back({ 0,-1 });
		offsetData[0].push_back({ 0,2 });
		offsetData[0].push_back({ 0,-1 });
		offsetData[0].push_back({ 0,2 });
		offsetData[1].push_back({ 0,-1 });
		offsetData[1].push_back({ 0,0 });
		offsetData[1].push_back({ 0,0 });
		offsetData[1].push_back({ -1,0 });
		offsetData[1].push_back({ 2,0 });
		offsetData[2].push_back({ -1,-1 });
		offsetData[2].push_back({ -1,1 });
		offsetData[2].push_back({ -1,-2 });
		offsetData[2].push_back({ 0,1 });
		offsetData[2].push_back({ 0,-2 });
		offsetData[3].push_back({ -1,0 });
		offsetData[3].push_back({ -1,0 });
		offsetData[3].push_back({ -1,0 });
		offsetData[3].push_back({ 1,0 });
		offsetData[3].push_back({ -2,0 });
	}
	else {
		for (int i = 0; i < 5; i++) {
			offsetData[0].push_back({ 0,0 });
			offsetData[1].push_back({ 1,0 });
			offsetData[2].push_back({ 1,-1 });
			offsetData[3].push_back({ 0,-1 });
		}
	}
}

void Block::rotateBlock()
{
	for (int i = 0; i < tileInfo.size(); i++) {
		if (tileInfo[i].X == 0 || tileInfo[i].Y == 0) {
			if (tileInfo[i].Y == 0) {
				tileInfo[i].X = tileInfo[i].X - 2 * tileInfo[i].X;
			}
			swap(tileInfo[i].X, tileInfo[i].Y);
		}
		else if ((tileInfo[i].X < 0 && tileInfo[i].Y < 0) || (tileInfo[i].X > 0 && tileInfo[i].Y > 0)) 
			tileInfo[i].Y = tileInfo[i].Y - 2 * tileInfo[i].Y;
		else 
			tileInfo[i].X = tileInfo[i].X - 2 * tileInfo[i].X;
	}
}

