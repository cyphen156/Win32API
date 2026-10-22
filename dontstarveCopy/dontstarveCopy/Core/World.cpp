#include "pch.h"
#include "World.h"
#include "CameraMgr.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

World::World(SceneType type)
    : wldSize(0, 0)
    , worldMap{}
	, tiles{}
    , tileSize(0)
    , spawnPos(0, 0)
{
}

World::~World() 
{
}

void World::SetWorldSize(bool isLarge)
{
    if (isLarge)
    {
        wldSize.x = WSLARGE;
        wldSize.y = WSLARGE;
    }
    else 
    {
        wldSize.x = WSREGULAR;
        wldSize.y = WSREGULAR;
    }
}

void World::generateWorld()
{
    const wchar_t* tileNames[(UINT)TileType::END] =
    {
        L"OceanCoastal", L"OceanSwell", L"OceanRough", L"OceanHazardous", L"Marsh", L"Rocky",
        L"YellowGrass", L"Forest", L"Grass", L"Dirt", L"Deciduous", L"DesertDirt",
    };
    for (UINT i = 0; i < (UINT)TileType::END; ++i)
    {
        wstring tileName = wstring(tileNames[i]) + L"_Default_000.png";
        wstring tilePath = PathMgr::GetInst()->GetContentPath();
        tilePath += L"\\Textures\\Terrain\\" + wstring(tileNames[i]) + L"\\Default\\" + tileName;
        tiles.push_back(ResourceMgr::GetInst()->LoadTexture(tileName, tilePath));
    }
	setTileSize(120);
	// 전체 월드 생성, default Biome : water
    worldMap.resize(wldSize.x, std::vector<Terrain>(wldSize.y, Terrain(BiomeType::Water, TileType::OceanCoastal)));
	// 기본 스폰 위치 : 월드 정중앙
	setSpawnPos(wldSize * Vec2D(tileSize, tileSize) / Vec2D(2, 2));
}

void World::Render(HDC hdc)
{
    if (worldMap.empty() || tileSize <= 0)
    {
        return;
    }

    CameraMgr* camera = CameraMgr::GetInst();
    Vec2D lookAt = camera->GetLookAt();
    Vec2D resolution = camera->GetResolution();
    float zoom = camera->GetZoom();
    float groundZoom = zoom * camera->GetPitchScale();
    int firstX = (int)floor((lookAt.x - resolution.x / 2.f / zoom) / tileSize);
    int lastX = (int)floor((lookAt.x + resolution.x / 2.f / zoom) / tileSize);
    int firstY = (int)floor((lookAt.y - resolution.y / 2.f / groundZoom) / tileSize);
    int lastY = (int)floor((lookAt.y + resolution.y / 2.f / groundZoom) / tileSize);

    Terrain outside(BiomeType::Water, TileType::OceanCoastal);
    SetStretchBltMode(hdc, COLORONCOLOR);
    for (int tileY = firstY; tileY <= lastY; ++tileY)
    {
        for (int tileX = firstX; tileX <= lastX; ++tileX)
        {
            Terrain* terrain = &outside;
            if (tileX >= 0 && tileX < (int)worldMap.size() && tileY >= 0 && tileY < (int)worldMap[tileX].size())
            {
                terrain = &worldMap[tileX][tileY];
            }
            Vec2D renderPos = camera->GetRenderPos(Vec2D(tileX * tileSize, tileY * tileSize));
            RECT tileRect = { (LONG)floor(renderPos.x), (LONG)floor(renderPos.y),
                (LONG)ceil(renderPos.x + tileSize * zoom), (LONG)ceil(renderPos.y + tileSize * groundZoom) };
            terrain->render(hdc, tileRect, tiles[(UINT)terrain->getTileType()]);
        }
    }
}
