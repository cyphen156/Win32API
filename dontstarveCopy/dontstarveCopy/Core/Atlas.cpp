#include "pch.h"
#include "Atlas.h"
#include "Texture.h"

Atlas::Atlas()
	: Sprite()
	, regions()
{
}

void Atlas::SetGrid(float cellSize)
{
	regions.clear();
	if (GetTexture() == nullptr || GetTexture()->GetImage() == nullptr || cellSize <= 0.f)
	{
		return;
	}

	int width = (int)GetTexture()->GetImage()->GetWidth();
	int height = (int)GetTexture()->GetImage()->GetHeight();
	int columns = max(1, (int)roundf(width / cellSize));
	int rows = max(1, (int)roundf(height / cellSize));
	regions.assign(rows, vector<RECT>(columns));
	for (int row = 0; row < rows; ++row)
	{
		for (int column = 0; column < columns; ++column)
		{
			regions[row][column] = { (LONG)roundf(column * cellSize), (LONG)roundf(row * cellSize), (LONG)min((float)width, roundf((column + 1) * cellSize)), (LONG)min((float)height, roundf((row + 1) * cellSize)) };
		}
	}
}

RECT Atlas::GetRegion(int column, int row)
{
	if (regions.empty())
	{
		if (GetTexture() == nullptr || GetTexture()->GetImage() == nullptr)
		{
			return {};
		}
		return { 0, 0, (LONG)GetTexture()->GetImage()->GetWidth(), (LONG)GetTexture()->GetImage()->GetHeight() };
	}

	return regions[row % regions.size()][column % regions[0].size()];
}