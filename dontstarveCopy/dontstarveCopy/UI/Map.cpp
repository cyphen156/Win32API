#include "pch.h"
#include "Map.h"
#include "Core.h"
#include "InputMgr.h"
#include "CameraMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "World.h"
#include "Tile.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "Texture.h"
#include "EventMgr.h"

static const float mapZoomStep = 1.25f;
static const float maxMapScale = 16.f;

Map::Map()
	: mapBrushes{}
	, mapBrushSize(0)
	, mapDC(nullptr)
	, mapBitmap(nullptr)
	, mapWidth(0)
	, mapHeight(0)
	, mapScale(1.f)
	, mapCenter{}
	, mapPixelsPerTile(1.f)
	, isMapChanged(true)
{
	wstring mapPath = PathMgr::GetInst()->GetContentPath();
	mapPath += L"\\Textures\\UI\\Map";
	ResourceMgr::GetInst()->LoadTexture(L"Map_Frame_000.png", mapPath + L"\\Frame\\Map_Frame_000.png");
	ResourceMgr::GetInst()->LoadTexture(L"Map_Cursor_000.png", mapPath + L"\\Cursor\\Map_Cursor_000.png");
}

Map::~Map()
{
	for (UINT i = 0; i < (UINT)TileType::END; ++i)
	{
		DeleteObject(mapBrushes[i]);
	}
	DeleteDC(mapDC);
	DeleteObject(mapBitmap);
}

void Map::update()
{
	World* world = SceneMgr::GetInst()->GetCurScene()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	// 펼칠 때 지도 가운데를 플레이어 자리로 되돌린다
	if (KEY_DOWN(KEY::TAB))
	{
		SetOpen(!IsOpen());
		if (IsOpen())
		{
			mapCenter = CameraMgr::GetInst()->GetLookAt();
			isMapChanged = true;
		}

		tEvent event = {};
		event.eEvent = EVENT_TYPE::PAUSE_GAME;
		event.lParam = (DWORD_PTR)IsOpen();
		EventMgr::GetInst()->AddEvent(event);
	}
	if (!IsOpen())
	{
		return;
	}

	// 휠은 배율(1 = 월드 전체가 들어감), 끌기는 중심을 옮긴다
	int wheel = InputMgr::GetInst()->GetMouseWheel();
	if (wheel > 0)
	{
		mapScale = min(mapScale * mapZoomStep, maxMapScale);
		isMapChanged = true;
	}
	else if (wheel < 0)
	{
		mapScale = max(mapScale / mapZoomStep, 1.f);
		isMapChanged = true;
	}

	if (KEY_PRESS(KEY::LBUTTON))
	{
		Vec2D delta = InputMgr::GetInst()->GetMouseDelta();
		mapCenter = Vec2D(mapCenter.x - delta.x / mapPixelsPerTile * TILESIZE, mapCenter.y - delta.y / mapPixelsPerTile * TILESIZE);
		isMapChanged = true;
	}
}

void Map::render(HDC hdc)
{
	World* world = SceneMgr::GetInst()->GetCurScene()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	POINT resolution = Core::GetInst()->GetResolution();
	int width = resolution.x;
	int height = resolution.y;

	Texture* frame = ResourceMgr::GetInst()->FindTexture(L"Map_Frame_000.png");
	SetStretchBltMode(hdc, HALFTONE);
	SetBrushOrgEx(hdc, 0, 0, nullptr);
	HDC frameDC = frame->GetDC();
	StretchBlt(hdc, 0, 0, width, height, frameDC, 0, 0, frame->Width(), frame->Height(), SRCCOPY);

	// 지도는 바탕 안쪽에 펼친다
	RECT area = { (LONG)(width * 0.05f), (LONG)(height * 0.09f), (LONG)(width * 0.95f), (LONG)(height * 0.91f) };
	int areaWidth = area.right - area.left;
	int areaHeight = area.bottom - area.top;
	Vec2D wldSize = world->getWorldSize();

	// 지도 이미지는 크기, 배율, 중심이 바뀔 때만 다시 그린다
	if (mapDC == nullptr || mapWidth != areaWidth || mapHeight != areaHeight || isMapChanged)
	{
		if (mapDC == nullptr)
		{
			mapDC = CreateCompatibleDC(hdc);
		}
		if (mapWidth != areaWidth || mapHeight != areaHeight)
		{
			HBITMAP newBitmap = CreateCompatibleBitmap(hdc, areaWidth, areaHeight);
			SelectObject(mapDC, newBitmap);
			DeleteObject(mapBitmap);
			mapBitmap = newBitmap;
			mapWidth = areaWidth;
			mapHeight = areaHeight;
		}
		isMapChanged = false;

		// 배율 1이면 월드 전체가 들어가고, 중심은 월드 밖으로 벗어나지 않는다
		float fitPixelsPerTile = min(areaWidth / wldSize.x, areaHeight / wldSize.y);
		float pixelsPerTile = fitPixelsPerTile * mapScale;
		float halfViewX = areaWidth / pixelsPerTile / 2.f;
		float halfViewY = areaHeight / pixelsPerTile / 2.f;
		float centerX = mapCenter.x / TILESIZE;
		float centerY = mapCenter.y / TILESIZE;
		centerX = halfViewX * 2.f >= wldSize.x ? wldSize.x / 2.f : min(max(centerX, halfViewX), wldSize.x - halfViewX);
		centerY = halfViewY * 2.f >= wldSize.y ? wldSize.y / 2.f : min(max(centerY, halfViewY), wldSize.y - halfViewY);
		mapCenter = Vec2D(centerX * TILESIZE, centerY * TILESIZE);
		mapPixelsPerTile = pixelsPerTile;

		float originX = areaWidth / 2.f - centerX * pixelsPerTile;
		float originY = areaHeight / 2.f - centerY * pixelsPerTile;

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
		SetBrushOrgEx(mapDC, ((int)floor(originX) % brushSize + brushSize) % brushSize, ((int)floor(originY) % brushSize + brushSize) % brushSize, nullptr);
		RECT imageRect = { 0, 0, areaWidth, areaHeight };
		FillRect(mapDC, &imageRect, mapBrushes[(UINT)TileType::OceanCoastal]);

		// 한 줄에서 같은 타일이 이어지는 칸은 사각형 하나로 칠한다
		vector<vector<Terrain>>& terrains = world->getTerrains();
		int firstX = max(0, (int)floor(-originX / pixelsPerTile));
		int lastX = min((int)wldSize.x - 1, (int)floor((areaWidth - 1 - originX) / pixelsPerTile));
		int firstY = max(0, (int)floor(-originY / pixelsPerTile));
		int lastY = min((int)wldSize.y - 1, (int)floor((areaHeight - 1 - originY) / pixelsPerTile));
		for (int tileY = firstY; tileY <= lastY; ++tileY)
		{
			LONG runTop = (LONG)floor(originY + tileY * pixelsPerTile);
			LONG runBottom = (LONG)floor(originY + (tileY + 1) * pixelsPerTile);
			int runStart = firstX;
			TileType runTile = terrains[firstX][tileY].getTileType();
			for (int tileX = firstX + 1; tileX <= lastX + 1; ++tileX)
			{
				if (tileX <= lastX && terrains[tileX][tileY].getTileType() == runTile)
				{
					continue;
				}
				RECT runRect = { (LONG)floor(originX + runStart * pixelsPerTile), runTop, (LONG)floor(originX + tileX * pixelsPerTile), runBottom };
				FillRect(mapDC, &runRect, mapBrushes[(UINT)runTile]);
				if (tileX <= lastX)
				{
					runStart = tileX;
					runTile = terrains[tileX][tileY].getTileType();
				}
			}
		}
		SetBrushOrgEx(mapDC, 0, 0, nullptr);
	}
	BitBlt(hdc, area.left, area.top, areaWidth, areaHeight, mapDC, 0, 0, SRCCOPY);

	Vec2D lookAt = CameraMgr::GetInst()->GetLookAt();
	Vec2D playerPos = Vec2D(areaWidth / 2.f + (lookAt.x - mapCenter.x) / TILESIZE * mapPixelsPerTile, areaHeight / 2.f + (lookAt.y - mapCenter.y) / TILESIZE * mapPixelsPerTile);
	Image* cursor = ResourceMgr::GetInst()->FindTexture(L"Map_Cursor_000.png")->GetImage();
	Graphics graphics(hdc);
	graphics.DrawImage(cursor, area.left + playerPos.x - cursor->GetWidth() / 2.f, area.top + playerPos.y - cursor->GetHeight() / 2.f, (REAL)cursor->GetWidth(), (REAL)cursor->GetHeight());
}
