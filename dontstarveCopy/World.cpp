#define NOMINMAX

#include "World.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define PI 3.14159265358979323846

World::World(RECT* wlSize, int tileSize)
    : worldWidth(wlSize->right)
    , worldHeight(wlSize->bottom), tileSize(tileSize) {

    // 2D 배열 초기화
    terrains = new Terrain*[worldWidth];
    for (int i = 0; i < worldWidth; ++i) {
        terrains[i] = new Terrain[worldHeight];
    }

    // 랜덤 시드 초기화
    srand(static_cast<unsigned>(time(0)));
    
}

World::~World() {
    for (int i = 0; i < worldWidth; ++i) {
        delete[] terrains[i];
    }
    delete[] terrains;
}

// worldSize의 중심좌표를 기준으로 방사형 월드를 생성
void World::generateWorld(POINT* wldCenterPos) {

    // 전체 월드 생성, default : water
    // 전체 월드를 water로 초기화
    for (int x = 0; x < worldWidth; ++x) {
        for (int y = 0; y < worldHeight; ++y) {
            terrains.push_back(new Terrain(x, y, tileSize, TerrainType::Water));
        }
    }


    // 8개의 필수 바이옴 생성
    std::vector<BiomeType> biomes;

    for (int i = 0; i < 8; ++i)
    {
        biomes.push_back(BiomeType(i));
    }

    // 랜덤 난수를 통한 0~4개의 바이옴 추가
    int addBiomes = rand() % 5;
    for (int i = 0; i < addBiomes; ++i) {
        int randomBiome = rand() % 8;
        biomes.push_back(static_cast<BiomeType>(randomBiome));
    }

    // 바이옴 순서를 섞기
    for (int i = 0; i < biomes.size(); ++i) {
        int randomIndex = rand() % biomes.size();
        std::swap(biomes[i], biomes[randomIndex]);
    }

    // 중심좌표
    int centerX = wldCenterPos->x;
    int centerY = wldCenterPos->y;

    // 바이옴을 방사형으로 배치
    float angleStep = 360.0f / biomes.size();
    float currentAngle = static_cast<float>(rand() % 360);

    // 땅의 비율
    int landRadius = std::min(worldWidth, worldHeight) * 0.35;
    int totalRadius = std::min(worldWidth, worldHeight) * 0.5;

    for (const auto& biome : biomes) {
        // 각도에 따라 좌표 계산
        int x = centerX + static_cast<int>(cos(currentAngle * (PI / 180.0f)) * landRadius);
        int y = centerY + static_cast<int>(sin(currentAngle * (PI / 180.0f)) * landRadius);

        // Terrain 객체 생성
        terrains.push_back(new Terrain(x, y, tileSize, biome));

        // 각도 업데이트
        currentAngle += angleStep;
    }
}