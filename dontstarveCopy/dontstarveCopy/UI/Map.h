#pragma once
#include "UI.h"
#include "Vec2D.h"

// 전체 지도. 지금 씬 월드의 지도를 배율과 중심에 맞춰 펼쳐 보여 준다
class Map : public UI
{
private:
	HBRUSH mapBrushes[(UINT)TileType::END];
	int mapBrushSize;
	HDC mapDC;
	HBITMAP mapBitmap;
	int mapWidth;
	int mapHeight;
	float mapScale;
	Vec2D mapCenter;
	float mapPixelsPerTile;
	bool isMapChanged;

public:
	Map();
	~Map();

	virtual void update();
	virtual void render(HDC hdc);
};
