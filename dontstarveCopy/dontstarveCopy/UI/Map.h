#pragma once
#include "UI.h"
#include "Vec2D.h"

class Map : public UI
{
private:
	HDC mapDC;
	HBITMAP mapBitmap;
	int mapWidth;
	int mapHeight;
	float mapScale;
	Vec2D mapCenter;
	float mapPixelsPerTile;
	bool isMapChanged;

protected:
	void OnOpen();
	void OnClose();

public:
	Map();
	~Map();

	virtual void update();
	virtual void render(HDC hdc);
};
