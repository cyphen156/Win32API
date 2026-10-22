#include "pch.h"
#include "MapRenderer.h"
#include "World.h"
#include "Tile.h"
#include "Texture.h"

MapRenderer::MapRenderer()
	: mapBrushes{}
	, mapBrushSize(0)
{
}

MapRenderer::~MapRenderer()
{
	for (UINT i = 0; i < (UINT)TileType::END; ++i)
	{
		DeleteObject(mapBrushes[i]);
	}
}

void MapRenderer::render(HDC hdc, const RECT& area, World* world, float originX, float originY, float pixelsPerTile)
{
	// 지도 무늬 한 장이 타일 4칸을 덮는다. 크기가 바뀌면 무늬를 그 크기로 다시 늘린다
	int brushSize = max(1, (int)lround(pixelsPerTile * 4.f));
	if (brushSize != mapBrushSize)
	{
		HDC patternDC = CreateCompatibleDC(hdc);
		SetStretchBltMode(patternDC, HALFTONE);
		SetBrushOrgEx(patternDC, 0, 0, nullptr);
		for (UINT i = 0; i < (UINT)TileType::END; ++i)
		{
			DeleteObject(mapBrushes[i]);
			HBITMAP pattern = CreateCompatibleBitmap(hdc, brushSize, brushSize);
			HBITMAP oldPattern = (HBITMAP)SelectObject(patternDC, pattern);
			Texture* mapTile = tileDefs[i].getMapTexture();
			HDC mapTileDC = mapTile->GetDC();
			StretchBlt(patternDC, 0, 0, brushSize, brushSize, mapTileDC, 0, 0, mapTile->Width(), mapTile->Height(), SRCCOPY);
			SelectObject(patternDC, oldPattern);
			mapBrushes[i] = CreatePatternBrush(pattern);
			DeleteObject(pattern);
		}
		DeleteDC(patternDC);
		mapBrushSize = brushSize;
	}

	// 무늬는 월드 원점에 맞춰 이어지고, 월드 밖은 물로 칠한다
	SetBrushOrgEx(hdc, ((int)floor(originX) % brushSize + brushSize) % brushSize, ((int)floor(originY) % brushSize + brushSize) % brushSize, nullptr);
	FillRect(hdc, &area, mapBrushes[(UINT)TileType::OceanCoastal]);

	// 한 줄에서 같은 타일이 이어지는 칸은 사각형 하나로 칠한다
	vector<vector<Terrain>>& terrains = world->getTerrains();
	Vec2D wldSize = world->getWorldSize();
	int firstX = max(0, (int)floor((area.left - originX) / pixelsPerTile));
	int lastX = min((int)wldSize.x - 1, (int)floor((area.right - 1 - originX) / pixelsPerTile));
	int firstY = max(0, (int)floor((area.top - originY) / pixelsPerTile));
	int lastY = min((int)wldSize.y - 1, (int)floor((area.bottom - 1 - originY) / pixelsPerTile));
	for (int tileY = firstY; tileY <= lastY && firstX <= lastX; ++tileY)
	{
		LONG runTop = max(area.top, (LONG)floor(originY + tileY * pixelsPerTile));
		LONG runBottom = min(area.bottom, (LONG)floor(originY + (tileY + 1) * pixelsPerTile));
		int runStart = firstX;
		TileType runTile = terrains[firstX][tileY].getTileType();
		for (int tileX = firstX + 1; tileX <= lastX + 1; ++tileX)
		{
			if (tileX <= lastX && terrains[tileX][tileY].getTileType() == runTile)
			{
				continue;
			}
			RECT runRect = { max(area.left, (LONG)floor(originX + runStart * pixelsPerTile)), runTop,
				min(area.right, (LONG)floor(originX + tileX * pixelsPerTile)), runBottom };
			FillRect(hdc, &runRect, mapBrushes[(UINT)runTile]);
			if (tileX <= lastX)
			{
				runStart = tileX;
				runTile = terrains[tileX][tileY].getTileType();
			}
		}
	}
	SetBrushOrgEx(hdc, 0, 0, nullptr);
}
