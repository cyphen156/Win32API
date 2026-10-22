#pragma once
#include "UI.h"
#include "Vec2D.h"
#include "MapRenderer.h"

// HUD의 미니맵. 플레이어를 가운데 두고 지도의 일부를 보여 준다
class Minimap : public UI
{
private:
	MapRenderer renderer;
	RECT frameRect;
	RECT mapRect;
	float pixelsPerTile;
	Vec2D offset;

public:
	Minimap();
	~Minimap();

	virtual void update();
	virtual void render(HDC hdc);
};
