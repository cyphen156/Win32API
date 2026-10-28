#include "pch.h"
#include "Minimap.h"
#include "Core.h"
#include "InputMgr.h"
#include "CameraMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "World.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "Texture.h"

static const float minimapWidthRatio = 0.225f;
static const float minimapMarginRatio = 0.01f;
static const float minimapZoomStep = 1.25f;
static const float minPixelsPerTile = 1.f;
static const float maxPixelsPerTile = 16.f;

Minimap::Minimap()
	: frameRect{}
	, mapRect{}
	, pixelsPerTile(4.f)
	, offset{}
{
	SetOpen(true);
	wstring mapPath = PathMgr::GetInst()->GetContentPath();
	mapPath += L"\\Textures\\UI\\Map";
	ResourceMgr::GetInst()->LoadTexture(L"Map_Frame_000.png", mapPath + L"\\Frame\\Map_Frame_000.png");
	ResourceMgr::GetInst()->LoadTexture(L"Map_Cursor_000.png", mapPath + L"\\Cursor\\Map_Cursor_000.png");
}

Minimap::~Minimap()
{
}

void Minimap::update()
{
	// 화면 오른쪽 위에 바탕 그림 비율대로 놓고, 지도는 바탕 안쪽에 펼친다
	POINT resolution = Core::GetInst()->GetResolution();
	Texture* frame = ResourceMgr::GetInst()->FindTexture(L"Map_Frame_000.png");
	LONG width = (LONG)(resolution.x * minimapWidthRatio);
	LONG height = (LONG)(width * (float)frame->Height() / frame->Width());
	LONG margin = (LONG)(resolution.x * minimapMarginRatio);
	frameRect = { resolution.x - margin - width, margin, resolution.x - margin, margin + height };
	mapRect = { frameRect.left + (LONG)(width * 0.05f), frameRect.top + (LONG)(height * 0.09f), frameRect.left + (LONG)(width * 0.95f), frameRect.top + (LONG)(height * 0.91f) };

	Vec2D mousePos = InputMgr::GetInst()->GetMousePos();
	if (mousePos.x < frameRect.left || mousePos.x >= frameRect.right || mousePos.y < frameRect.top || mousePos.y >= frameRect.bottom)
	{
		return;
	}

	// 마우스가 올라와 있으면 휠은 배율, 끌기는 옮기기로 미니맵이 가져간다
	int wheel = InputMgr::GetInst()->GetMouseWheel();
	if (wheel > 0)
	{
		pixelsPerTile = min(pixelsPerTile * minimapZoomStep, maxPixelsPerTile);
	}
	else if (wheel < 0)
	{
		pixelsPerTile = max(pixelsPerTile / minimapZoomStep, minPixelsPerTile);
	}
	InputMgr::GetInst()->ConsumeMouseWheel();

	if (KEY_PRESS(KEY::LBUTTON))
	{
		Vec2D delta = InputMgr::GetInst()->GetMouseDelta();
		offset = Vec2D(offset.x - delta.x / pixelsPerTile * TILESIZE, offset.y - delta.y / pixelsPerTile * TILESIZE);
	}
	InputMgr::GetInst()->ConsumeKey(KEY::LBUTTON);
}

void Minimap::render(HDC hdc)
{
	World* world = SceneMgr::GetInst()->GetCurScene()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	Texture* frame = ResourceMgr::GetInst()->FindTexture(L"Map_Frame_000.png");
	SetStretchBltMode(hdc, HALFTONE);
	SetBrushOrgEx(hdc, 0, 0, nullptr);
	HDC frameDC = frame->GetDC();
	StretchBlt(hdc, frameRect.left, frameRect.top, frameRect.right - frameRect.left, frameRect.bottom - frameRect.top, frameDC, 0, 0, frame->Width(), frame->Height(), SRCCOPY);

	// 가운데는 플레이어 자리에 끌어 옮긴 만큼을 더한 곳이다
	Vec2D lookAt = CameraMgr::GetInst()->GetLookAt();
	float originX = (mapRect.left + mapRect.right) / 2.f - (lookAt.x + offset.x) / TILESIZE * pixelsPerTile;
	float originY = (mapRect.top + mapRect.bottom) / 2.f - (lookAt.y + offset.y) / TILESIZE * pixelsPerTile;

	float playerX = originX + lookAt.x / TILESIZE * pixelsPerTile;
	float playerY = originY + lookAt.y / TILESIZE * pixelsPerTile;
	if (playerX < mapRect.left || playerX >= mapRect.right || playerY < mapRect.top || playerY >= mapRect.bottom)
	{
		return;
	}
	Image* cursor = ResourceMgr::GetInst()->FindTexture(L"Map_Cursor_000.png")->GetImage();
	Graphics graphics(hdc);
	graphics.DrawImage(cursor, playerX - cursor->GetWidth() / 2.f, playerY - cursor->GetHeight() / 2.f, (REAL)cursor->GetWidth(), (REAL)cursor->GetHeight());
}
