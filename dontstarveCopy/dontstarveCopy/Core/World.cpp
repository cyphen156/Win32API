#include "pch.h"
#include "World.h"
#include "CameraMgr.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

const WorldDef worldDefs[END] =
{
	{ 
        Forest, 
        { 
            BiomeType::Grass, 
            BiomeType::Forest, 
            BiomeType::Savanna, 
            BiomeType::Deciduous, 
            BiomeType::Marsh, 
            BiomeType::Desert, 
            BiomeType::MandrakeForest, 
            BiomeType::Rocky 
        } 
    },
	{ 
        Cave, 
        { 
        } 
    },
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
    terrains.resize(wldSize.x);
    for (int x = 0; x < wldSize.x; ++x)
    {
        terrains[x].reserve(wldSize.y);
        for (int y = 0; y < wldSize.y; ++y)
        {
            terrains[x].push_back(Terrain(BiomeType::Water, TileType::OceanCoastal, Vec2D(x, y)));
        }
    }
    setSpawnPos(wldSize * Vec2D(TILESIZE, TILESIZE) / Vec2D(2, 2));
}

void World::Render(HDC hdc)
{
    if (terrains.empty())
    {
        return;
    }

    SetStretchBltMode(hdc, COLORONCOLOR);

	RECT view = CameraMgr::GetInst()->GetViewArea();    // 카메라 기준 절두 영역 확보 및 컬링
    int firstX = max((int)floor((float)view.left / TILESIZE), 0);
    int lastX = min((int)ceil((float)view.right / TILESIZE), (int)wldSize.x);
    int firstY = max((int)floor((float)view.top / TILESIZE), 0);
    int lastY = min((int)ceil((float)view.bottom / TILESIZE), (int)wldSize.y);
    for (int x = firstX; x < lastX; ++x)
    {
        for (int y = firstY; y < lastY; ++y)
        {
            terrains[x][y].render(hdc);
        }
    }
}
