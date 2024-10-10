#define NOMINMAX

#include "World.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define PI 3.14159265358979323846

World::World(RECT* wlSize, int tileSize)
    : worldWidth(wlSize->right)
    , worldHeight(wlSize->bottom), tileSize(tileSize) {

    // 2D �迭 �ʱ�ȭ
    terrains = new Terrain*[worldWidth];
    for (int i = 0; i < worldWidth; ++i) {
        terrains[i] = new Terrain[worldHeight];
    }

    // ���� �õ� �ʱ�ȭ
    srand(static_cast<unsigned>(time(0)));
    
}

World::~World() {
    for (int i = 0; i < worldWidth; ++i) {
        delete[] terrains[i];
    }
    delete[] terrains;
}

// worldSize�� �߽���ǥ�� �������� ����� ���带 ����
void World::generateWorld(POINT* wldCenterPos) {

    // ��ü ���� ����, default : water
    // ��ü ���带 water�� �ʱ�ȭ
    for (int x = 0; x < worldWidth; ++x) {
        for (int y = 0; y < worldHeight; ++y) {
            terrains.push_back(new Terrain(x, y, tileSize, TerrainType::Water));
        }
    }


    // 8���� �ʼ� ���̿� ����
    std::vector<BiomeType> biomes;

    for (int i = 0; i < 8; ++i)
    {
        biomes.push_back(BiomeType(i));
    }

    // ���� ������ ���� 0~4���� ���̿� �߰�
    int addBiomes = rand() % 5;
    for (int i = 0; i < addBiomes; ++i) {
        int randomBiome = rand() % 8;
        biomes.push_back(static_cast<BiomeType>(randomBiome));
    }

    // ���̿� ������ ����
    for (int i = 0; i < biomes.size(); ++i) {
        int randomIndex = rand() % biomes.size();
        std::swap(biomes[i], biomes[randomIndex]);
    }

    // �߽���ǥ
    int centerX = wldCenterPos->x;
    int centerY = wldCenterPos->y;

    // ���̿��� ��������� ��ġ
    float angleStep = 360.0f / biomes.size();
    float currentAngle = static_cast<float>(rand() % 360);

    // ���� ����
    int landRadius = std::min(worldWidth, worldHeight) * 0.35;
    int totalRadius = std::min(worldWidth, worldHeight) * 0.5;

    for (const auto& biome : biomes) {
        // ������ ���� ��ǥ ���
        int x = centerX + static_cast<int>(cos(currentAngle * (PI / 180.0f)) * landRadius);
        int y = centerY + static_cast<int>(sin(currentAngle * (PI / 180.0f)) * landRadius);

        // Terrain ��ü ����
        terrains.push_back(new Terrain(x, y, tileSize, biome));

        // ���� ������Ʈ
        currentAngle += angleStep;
    }
}