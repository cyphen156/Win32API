#include "pch.h"
#include "Map.h"
#include "Core.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "World.h"
#include "ResourceMgr.h"
#include "Texture.h"
#include "EnumString.h"
#include "PathMgr.h"
#include "CameraMgr.h"
#include "InputMgr.h"
#include "Monster.h"

const int pixelperTile = 16;	// 16*16pixels per tile, use 256*256 pixel image slices for each tile type
static const float maxZoomScale = 4.f;
static const int maxZoomLevel = (int)roundf(2.f * log2f(maxZoomScale));
static const int minZoomLevel = (int)roundf(-2.f * log2f((float)pixelperTile));


Map::Map()
	: UI(L"Map")
	, zoomLevel(0)
	, groundDC(nullptr)
	, frameDC(nullptr)
{
	wstring framePath = PathMgr::GetInst()->GetContentPath();
	framePath += L"\\Textures\\UI\\Map\\Frame\\Map_Frame_000.png";
	frame.SetTexture(ResourceMgr::GetInst()->LoadTexture(L"Map_Frame_000.png", framePath));
	frame.GetTransform().parent = &transform;

	drawTransformer.parent = &transform;

	int levelCount = maxZoomLevel - minZoomLevel + 1;

	cachedTileAtlases.assign(levelCount, vector<Atlas*>((UINT)TileType::END, nullptr));
	cachedIcons.assign(levelCount, vector<Sprite*>((UINT)MonsterType::END + 1, nullptr));
}

Map::~Map()
{
	for (size_t index = 0; index < cachedTileAtlases.size(); ++index)
	{
		Safe_Delete_Vec<Atlas*>(cachedTileAtlases[index]);
		Safe_Delete_Vec<Sprite*>(cachedIcons[index]);
	}

	if (groundDC != nullptr)
	{
		HBITMAP groundBitmap = (HBITMAP)GetCurrentObject(groundDC, OBJ_BITMAP);
		DeleteDC(groundDC);
		DeleteObject(groundBitmap);
	}

	if (frameDC != nullptr)
	{
		HBITMAP frameBitmap = (HBITMAP)GetCurrentObject(frameDC, OBJ_BITMAP);
		DeleteDC(frameDC);
		DeleteObject(frameBitmap);
	}
}

void Map::OnOpen()
{
	Vec2D lookAt = CameraMgr::GetInst()->GetLookAt();
	drawTransformer.position = Vec2D(
		-lookAt.x / TILESIZE * pixelperTile * drawTransformer.scale.x, 
		-lookAt.y / TILESIZE * pixelperTile * drawTransformer.scale.y);
	
	BakeFrame();
	BakeGround();
}

void Map::BakeGround()
{
	Scene* scene = SceneMgr::GetInst()->GetCurScene();
	if (scene == nullptr || scene->GetWorld() == nullptr)
	{
		return;
	}
	World* world = scene->GetWorld();

	Image* frameImage = frame.GetTexture()->GetImage();
	if (frameImage == nullptr)
	{
		return;
	}

	Transform mapFinal = transform.GetFinalTransform();
	int width = (int)roundf(frameImage->GetWidth() * mapFinal.scale.x);
	int height = (int)roundf(frameImage->GetHeight() * mapFinal.scale.y);

	BITMAPINFO info = {};
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = -height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;

	void* pixels = nullptr;
	HBITMAP bakedBitmap = CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, &pixels, nullptr, 0);
	if (bakedBitmap == nullptr)
	{
		return;
	}

	HDC bakedDC = CreateCompatibleDC(nullptr);
	if (bakedDC == nullptr)
	{
		DeleteObject(bakedBitmap);
		return;
	}
	SelectObject(bakedDC, bakedBitmap);
	SetStretchBltMode(bakedDC, COLORONCOLOR);

	Transform drawFinal = drawTransformer.GetFinalTransform();
	float cellWidth = pixelperTile * drawFinal.scale.x;
	float cellHeight = pixelperTile * drawFinal.scale.y;
	float originX = (float)(width / 2) + drawFinal.position.x - roundf(mapFinal.position.x);
	float originY = (float)(height / 2) + drawFinal.position.y - roundf(mapFinal.position.y);

	Vec2D worldSize = world->getWorldSize();
	int firstX = max(0, (int)floorf(-originX / cellWidth - 0.5f));
	int lastX = min((int)worldSize.x - 1, (int)ceilf((width - originX) / cellWidth + 0.5f));
	int firstY = max(0, (int)floorf(-originY / cellHeight - 0.5f));
	int lastY = min((int)worldSize.y - 1, (int)ceilf((height - originY) / cellHeight + 0.5f));

	vector<vector<Terrain>>& terrains = world->getTerrains();
	int index = maxZoomLevel - zoomLevel;
	for (int x = firstX; x <= lastX; ++x)
	{
		int left = (int)roundf(originX + x * cellWidth - cellWidth / 2.f);
		int right = (int)roundf(originX + x * cellWidth + cellWidth / 2.f);
		for (int y = firstY; y <= lastY; ++y)
		{
			Atlas* atlas = cachedTileAtlases[index][(UINT)terrains[x][y].getTileType()];
			if (atlas == nullptr)
			{
				continue;
			}

			int top = (int)roundf(originY + y * cellHeight - cellHeight / 2.f);
			int bottom = (int)roundf(originY + y * cellHeight + cellHeight / 2.f);
			RECT region = atlas->GetRegion(x, y);
			StretchBlt(bakedDC, left, top, right - left, bottom - top,
				atlas->GetTexture()->GetDC(), region.left, region.top, region.right - region.left, region.bottom - region.top, SRCCOPY);
		}
	}

	DrawIcons(bakedDC, Vec2D(originX, originY), Vec2D(cellWidth, cellHeight), Vec2D((float)width, (float)height));

	if (groundDC != nullptr)
	{
		HBITMAP groundBitmap = (HBITMAP)GetCurrentObject(groundDC, OBJ_BITMAP);
		DeleteDC(groundDC);
		DeleteObject(groundBitmap);
	}
	groundDC = bakedDC;
}

void Map::BakeFrame()
{
	Transform frameFinal = frame.GetTransform().GetFinalTransform();
	Image* frameImage = frame.GetTexture()->GetImage();
	if (frameImage == nullptr)
	{
		return;
	}

	int width = (int)roundf(frameImage->GetWidth() * frameFinal.scale.x);
	int height = (int)roundf(frameImage->GetHeight() * frameFinal.scale.y);

	BITMAPINFO info = {};
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = -height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;

	void* pixels = nullptr;
	HBITMAP bakedBitmap = CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, &pixels, nullptr, 0);
	if (bakedBitmap == nullptr)
	{
		return;
	}

	Status bakeStatus = GenericError;
	{
		Bitmap bakedFrame(width, height, width * 4, PixelFormat32bppPARGB, (BYTE*)pixels);
		Graphics graphics(&bakedFrame);
		graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
		bakeStatus = graphics.DrawImage(frameImage, 0, 0, width, height);
	}

	HDC bakedDC = nullptr;
	if (bakeStatus == Ok)
	{
		bakedDC = CreateCompatibleDC(nullptr);
	}
	if (bakedDC == nullptr)
	{
		DeleteObject(bakedBitmap);
		return;
	}
	SelectObject(bakedDC, bakedBitmap);

	if (frameDC != nullptr)
	{
		HBITMAP frameBitmap = (HBITMAP)GetCurrentObject(frameDC, OBJ_BITMAP);
		DeleteDC(frameDC);
		DeleteObject(frameBitmap);
	}
	frameDC = bakedDC;
}

void Map::DrawIcons(HDC targetDC, Vec2D origin, Vec2D cellSize, Vec2D targetSize)
{
	Scene* scene = SceneMgr::GetInst()->GetCurScene();
	if (scene == nullptr)
	{
		return;
	}

	Graphics graphics(targetDC);
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeHalf);

	int index = maxZoomLevel - zoomLevel;
	ObjType iconDrawOrder[] = { ObjType::Monster, ObjType::Player };
	for (ObjType group : iconDrawOrder)
	{
		const vector<Object*>& objects = scene->GetObjectGroup(group);
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (objects[i]->isDead())
			{
				continue;
			}

			UINT iconType = group == ObjType::Player ? (UINT)MonsterType::END : (UINT)static_cast<Monster*>(objects[i])->GetMonsterType();
			Sprite* icon = cachedIcons[index][iconType];
			if (icon == nullptr)
			{
				continue;
			}

			Image* iconImage = icon->GetTexture()->GetImage();
			Vec2D iconScale = icon->GetTransform().scale;
			int iconWidth = (int)roundf(iconImage->GetWidth() * iconScale.x * cellSize.x);
			int iconHeight = (int)roundf(iconImage->GetHeight() * iconScale.y * cellSize.y);

			Vec2D pos = objects[i]->GetPos();
			int left = (int)roundf(origin.x + pos.x / TILESIZE * cellSize.x) - iconWidth / 2;
			int top = (int)roundf(origin.y + pos.y / TILESIZE * cellSize.y) - iconHeight / 2;
			if (left + iconWidth < 0 || left > targetSize.x || top + iconHeight < 0 || top > targetSize.y)
			{
				continue;
			}

			graphics.DrawImage(iconImage, left, top, iconWidth, iconHeight);
		}
	}
}

void Map::update()
{

}

void Map::render(HDC hdc)
{
	// 1. Draw the drawing canvas (groundDC): baked by BakeGround on open, zoom, drag, resize
	//    1-1. Terrain tiles: cache of the current zoomLevel, stamped 1:1 at their centers
	//    1-2. Icons on top: DrawIcons with the icon cache of the current zoomLevel, skip dead objects, player last
	//
	// 2. Draw the frame (frameDC): baked by BakeFrame on open, resize, from the frame Transform
	//
	// Output: put the drawing canvas at the Map Transform's center (BitBlt 1:1), then the frame on top (AlphaBlend)

	Image* frameImage = frame.GetTexture()->GetImage();
	if (frameImage == nullptr)
	{
		return;
	}

	Transform mapFinal = transform.GetFinalTransform();
	int width = (int)roundf(frameImage->GetWidth() * mapFinal.scale.x);
	int height = (int)roundf(frameImage->GetHeight() * mapFinal.scale.y);

	if (groundDC != nullptr)
	{
		BitBlt(hdc, (int)roundf(mapFinal.position.x) - width / 2, (int)roundf(mapFinal.position.y) - height / 2, width, height, groundDC, 0, 0, SRCCOPY);
	}

	if (frameDC != nullptr)
	{
		Transform frameFinal = frame.GetTransform().GetFinalTransform();
		int frameWidth = (int)roundf(frameImage->GetWidth() * frameFinal.scale.x);
		int frameHeight = (int)roundf(frameImage->GetHeight() * frameFinal.scale.y);
		BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		AlphaBlend(hdc, (int)roundf(frameFinal.position.x) - frameWidth / 2, (int)roundf(frameFinal.position.y) - frameHeight / 2, frameWidth, frameHeight, frameDC, 0, 0, frameWidth, frameHeight, blend);
	}
}

void Map::Execute(const InputContext& ctx)
{
	int previousZoomLevel = zoomLevel;
	if (ctx.mouse.wheel > 0)
	{
		zoomLevel = min(maxZoomLevel, zoomLevel + 1);
	}
	else if (ctx.mouse.wheel < 0)
	{
		zoomLevel = max(minZoomLevel, zoomLevel - 1);
	}

	if (zoomLevel != previousZoomLevel)
	{
		float zoomScale = powf(2.f, zoomLevel * 0.5f);
		float ratio = zoomScale / drawTransformer.scale.y;
		drawTransformer.scale = Vec2D(drawTransformer.scale.x * ratio, zoomScale);
		drawTransformer.position = drawTransformer.position * Vec2D(ratio, ratio);
		BakeGround();
	}

	if (ctx.keys[(int)KEY::LBUTTON].eState == KEY_STATE::PRESS)
	{
		Transform mapFinal = transform.GetFinalTransform();
		drawTransformer.position += Vec2D(ctx.mouse.delta.x / mapFinal.scale.x, ctx.mouse.delta.y / mapFinal.scale.y);
		BakeGround();
	}
}

void Map::Refresh()
{
	// Load the map textures for each tile type from the resource manager, using the naming convention "Map_<TileName>_000.png"
	for (UINT i = 0; i < (UINT)TileType::END; ++i)
	{
		wstring tileName = EnumString::ToWString((TileType)i);
		wstring originKey = L"Map_" + tileName + L"_000.png";
		Texture* origin = ResourceMgr::GetInst()->FindTexture(originKey);
		if (origin == nullptr || origin->GetImage() == nullptr)
		{
			continue;
		}

		// Create cached map textures for each zoom level, using the original texture scaled to the cell size of each level
		float originScale = 1.f / pixelperTile;
		for (int level = minZoomLevel; level <= maxZoomLevel; ++level)
		{
			int index = maxZoomLevel - level;
			if (cachedTileAtlases[index][i] != nullptr)
			{
				continue;
			}

			float tileSize = pixelperTile * powf(2.f, level * 0.5f);
			UINT width = (UINT)max(1.f, roundf(origin->GetImage()->GetWidth() * originScale * tileSize));
			UINT height = (UINT)max(1.f, roundf(origin->GetImage()->GetHeight() * originScale * tileSize));

			Atlas* atlas = new Atlas;
			atlas->SetTexture(ResourceMgr::GetInst()->CreateResource<Texture>(originKey + L"@L" + to_wstring(level), origin, width, height));
			atlas->GetTransform().scale = Vec2D(1.f / tileSize, 1.f / tileSize);
			atlas->SetGrid(tileSize);
			cachedTileAtlases[index][i] = atlas;
		}
	}

	// Load the player and monster icons for each type from the resource manager, using the naming convention "Icon_<MonsterName>_000.png"
	wstring iconPath = PathMgr::GetInst()->GetContentPath();
	iconPath += L"\\Textures\\UI\\Icon\\";
	wstring playerKey = L"Icon_Wilson_000.png";
	Texture* playerOrigin = ResourceMgr::GetInst()->LoadTexture(playerKey, iconPath + L"Wilson\\" + playerKey);
	if (playerOrigin->GetImage() != nullptr)
	{
		float originScale = 2.f / (pixelperTile * maxZoomScale);

		// Create cached player icons for each zoom level, using the original texture and scaling it down to the appropriate size for each level
		for (int level = minZoomLevel; level <= maxZoomLevel; ++level)
		{
			int index = maxZoomLevel - level;
			if (cachedIcons[index][(UINT)MonsterType::END] != nullptr)
			{
				continue;
			}

			float tileSize = pixelperTile * powf(2.f, level * 0.5f);
			UINT width = (UINT)max(1.f, roundf(playerOrigin->GetImage()->GetWidth() * originScale * tileSize));
			UINT height = (UINT)max(1.f, roundf(playerOrigin->GetImage()->GetHeight() * originScale * tileSize));

			Sprite* icon = new Sprite;
			icon->SetTexture(ResourceMgr::GetInst()->CreateResource<Texture>(playerKey + L"@L" + to_wstring(level), playerOrigin, width, height));
			icon->GetTransform().scale = Vec2D(1.f / tileSize, 1.f / tileSize);
			cachedIcons[index][(UINT)MonsterType::END] = icon;
		}
	}

	for (UINT i = 0; i < (UINT)MonsterType::END; ++i)
	{
		wstring monsterName = EnumString::ToWString((MonsterType)i);
		wstring originKey = L"Icon_" + monsterName + L"_000.png";
		Texture* origin = ResourceMgr::GetInst()->LoadTexture(originKey, iconPath + monsterName + L"\\" + originKey);
		if (origin->GetImage() == nullptr)
		{
			continue;
		}

		// Create cached monster icons for each zoom level, using the original texture and scaling it down to the appropriate size for each level
		float originScale = 2.f / (pixelperTile * maxZoomScale);
		for (int level = minZoomLevel; level <= maxZoomLevel; ++level)
		{
			int index = maxZoomLevel - level;
			if (cachedIcons[index][i] != nullptr)
			{
				continue;
			}

			float tileSize = pixelperTile * powf(2.f, level * 0.5f);
			UINT width = (UINT)max(1.f, roundf(origin->GetImage()->GetWidth() * originScale * tileSize));
			UINT height = (UINT)max(1.f, roundf(origin->GetImage()->GetHeight() * originScale * tileSize));

			Sprite* icon = new Sprite;
			icon->SetTexture(ResourceMgr::GetInst()->CreateResource<Texture>(originKey + L"@L" + to_wstring(level), origin, width, height));
			icon->GetTransform().scale = Vec2D(1.f / tileSize, 1.f / tileSize);
			cachedIcons[index][i] = icon;
		}
	}
}

void Map::Resize()
{
	Transform mapFinal = transform.GetFinalTransform();

	float matchedScaleX = drawTransformer.scale.y * mapFinal.scale.y / mapFinal.scale.x;
	drawTransformer.position.x *= matchedScaleX / drawTransformer.scale.x;
	drawTransformer.scale.x = matchedScaleX;

	if (isOpen)
	{
		BakeFrame();
		BakeGround();
	}
}