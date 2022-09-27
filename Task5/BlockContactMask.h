#pragma once
#include<vector>
#include"Block.h"
using namespace std;

class BlockContactMask
{
public:
	BlockContactMask(BlockType type)
	{
		Initialize(type);
	}

	void Initialize(BlockType type)
	{
		switch (type)
		{
		case Z1:
			Data =
			{
				{ false, true, true, false, false },
				{ true, false, false, true, false },
				{ false, true, false, false, true },
				{ false, false, true, true, false }
			};
			StartPoint = { -2, -2 };
			break;
		case Z2:
			Data =
			{
				{ false, false, true, true, false },
				{ false, true, false, false, true },
				{ true, false, false, true, false },
				{ false, true, true, false, false }
			};
			StartPoint = { -2, -2 };
			break;
		case L1:
			Data =
			{
				{ false, false, false, true, false },
				{ false, true, true, false, true },
				{ true, false, false, false, true },
				{ false, true, true, true, false }
			};
			StartPoint = { -2, -2 };
			break;
		case L2:
			Data =
			{
				{ false, true, false, false, false },
				{ true, false, true, true, false },
				{ true, false, false, false, true },
				{ false, true, true, true, false }
			};
			StartPoint = { -2, -2 };
			break;
		case T:
			Data =
			{
				{ false, false, true, false, false },
				{ false, true, false, true, false },
				{ true, false, false, false, true },
				{ false, true, true, true, false }
			};
			StartPoint = { -2, -2 };
			break;
		case I:
			Data =
			{
				{ false, true, true, true, true, false },
				{ true, false, false, false, false, true },
				{ false, true, true, true, true, false }
			};
			StartPoint = { -1, -2 };
			break;
		case O:
			Data =
			{
				{ false, true, true, false },
				{ true, false, false, true },
				{ true, false, false, true },
				{ false, true, true, false }
			};
			StartPoint = { -2, -1 };
			break;
		}
	}

	void RotateData()
	{
		vector<vector<bool>> afterData(Data[0].size(), vector<bool>(Data.size()));

		for (int i = 0; i < Data.size(); i++){
			for (int j = 0; j < Data[i].size(); j++){
				afterData[j][Data.size() - i - 1] = Data[i][j];
			}
		}

		Data = afterData;
	}

	vector<vector<bool>> Data;
	pair<int, int> StartPoint;
};