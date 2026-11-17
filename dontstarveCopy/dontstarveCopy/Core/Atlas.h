#pragma once
#include "Sprite.h"

class Atlas : public Sprite
{
private:
	vector<vector<RECT>> regions;

public:
	Atlas();

	void SetGrid(float cellSize);
	RECT GetRegion(int column, int row);
};