#pragma once

class World;

// 월드 지형을 지도 무늬로 칠한다. 배율이 바뀌면 무늬 브러시를 그 크기로 다시 만든다
class MapRenderer
{
private:
	HBRUSH mapBrushes[(UINT)TileType::END];
	int mapBrushSize;

public:
	MapRenderer();
	~MapRenderer();

	void render(HDC hdc, const RECT& area, World* world, float originX, float originY, float pixelsPerTile);
};
