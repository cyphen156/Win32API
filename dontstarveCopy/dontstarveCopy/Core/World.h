#pragma once

#include "Vec2D.h"
#include "Terrain.h"

enum WorldType
{
	Forest,
	Cave,

	END
};

// 세계 타입에 들어가는 바이옴, 첫 번째가 시작 바이옴
struct WorldDef
{
	WorldType type;
	vector<BiomeType> biomes;
};

// WorldType 순서
extern const WorldDef worldDefs[END];

class World {
private:
    WorldType worldType;
    Vec2D wldSize;
    vector<vector<Terrain>> terrains;
    Vec2D spawnPos;

public:
    World(WorldType type);
    ~World();

    // 월드 크기 지정
    
    void SetWorldSize(bool isLarge);
    void setSpawnPos(Vec2D pos) { spawnPos = pos; }

    void Render(HDC hdc);

    WorldType getWorldType() { return worldType; }
    Vec2D getWorldSize() { return wldSize; }
    Vec2D getSpawnPos() { return spawnPos; }
    vector<vector<Terrain>>& getTerrains() { return terrains; }
};
