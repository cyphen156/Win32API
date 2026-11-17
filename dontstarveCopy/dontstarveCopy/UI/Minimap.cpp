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
	: UI(L"Minimap")
{
	SetOpen(true);
}

Minimap::~Minimap()
{
}

void Minimap::update()
{
}

void Minimap::render(HDC hdc)
{
}
