#include "pch.h"
#include "World.h"
#include "CameraMgr.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

const WorldDef worldDefs[END] =
{
	{ Forest, { BiomeType::Grass, BiomeType::Forest, BiomeType::Savanna, BiomeType::Deciduous, BiomeType::Marsh, BiomeType::Desert, BiomeType::MandrakeForest, BiomeType::Rocky } },
	{ Cave, { } },
};

World::World(WorldType type)
    : worldType(type)
    , wldSize(0, 0)
    , terrains{}
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
	// 전체 월드 생성, default Biome : water
    terrains.resize(wldSize.x, std::vector<Terrain>(wldSize.y, Terrain(BiomeType::Water, TileType::OceanCoastal)));
	// 기본 스폰 위치 : 월드 정중앙
	setSpawnPos(wldSize * Vec2D(TILESIZE, TILESIZE) / Vec2D(2, 2));
}

void World::Render(HDC hdc)
{
    if (terrains.empty())
    {
        return;
    }

    CameraMgr* camera = CameraMgr::GetInst();
    Vec2D lookAt = camera->GetLookAt();
    Vec2D resolution = camera->GetResolution();
    float zoom = camera->GetZoom();
    float groundZoom = zoom * camera->GetPitchScale();
    int firstX = (int)floor((lookAt.x - resolution.x / 2.f / zoom) / TILESIZE);
    int lastX = (int)floor((lookAt.x + resolution.x / 2.f / zoom) / TILESIZE);
    int firstY = (int)floor((lookAt.y - resolution.y / 2.f / groundZoom) / TILESIZE);
    int lastY = (int)floor((lookAt.y + resolution.y / 2.f / groundZoom) / TILESIZE);

    Terrain outside(BiomeType::Water, TileType::OceanCoastal);
    SetStretchBltMode(hdc, COLORONCOLOR);
    for (int tileY = firstY; tileY <= lastY; ++tileY)
    {
        for (int tileX = firstX; tileX <= lastX; ++tileX)
        {
            Terrain* terrain = &outside;
            if (tileX >= 0 && tileX < (int)terrains.size() && tileY >= 0 && tileY < (int)terrains[tileX].size())
            {
                terrain = &terrains[tileX][tileY];
            }
            Vec2D renderPos = camera->GetRenderPos(Vec2D(tileX * TILESIZE, tileY * TILESIZE));
            RECT tileRect = { (LONG)floor(renderPos.x), (LONG)floor(renderPos.y),
                (LONG)ceil(renderPos.x + TILESIZE * zoom), (LONG)ceil(renderPos.y + TILESIZE * groundZoom) };
            terrain->render(hdc, tileRect);
        }
    }
}
