#include "pch.h"
#include "World.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>

#define PI 3.14159265358979323846

static int cnt = 0;

World::World(RECT* wlSize) : worldWidth(wlSize->right), worldHeight(wlSize->bottom) 
{
    tileSize = 50;
    // ��ü ���� ����, default Biome : water
    worldMap.resize(worldWidth, std::vector<Terrain>(worldHeight, BiomeType::Water));

    // ���� �õ� �ʱ�ȭ
    srand(static_cast<unsigned>(time(0)));
}

World::~World() 
{
}


//int calcDist(const POINT& a, const POINT& b)
//{
//    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
//}
//
//// Prim �˰��� MST
//vector<pair<int, int>> generateMST(const vector<POINT>& biomeCenters)
//{
//    int biomecnt = biomeCenters.size();
//    vector<bool> isInMST(biomecnt, false);
//    vector<int> minDistance(biomecnt, INT_MAX);
//    vector<int> prev(biomecnt, -1);
//    vector<pair<int, int>> mstEdges;
//
//    minDistance[0] = 0;
//
//    for (int iter = 0; iter < biomecnt - 1; ++iter)
//    {
//        int currBiome = -1;
//        for (int i = 0; i < biomecnt; ++i)
//        {
//            if (!isInMST[i] && (currBiome == -1 || minDistance[i] < minDistance[currBiome]))
//            {
//                currBiome = i;
//            }
//        }
//
//        isInMST[currBiome] = true;
//
//        if (prev[currBiome] != -1)
//        {
//            mstEdges.push_back({ prev[currBiome], currBiome });
//        }
//
//        // �Ÿ� ����
//        for (int next = 0; next < biomecnt; ++next)
//        {
//            if (!isInMST[next])
//            {
//                int distance = calcDist(biomeCenters[currBiome], biomeCenters[next]);
//                if (distance < minDistance[next])
//                {
//                    minDistance[next] = distance;
//                    prev[next] = currBiome;
//                }
//            }
//        }
//    }
//
//    return mstEdges;
//}
//
//void bfsExpandBiome(const POINT& center, int& biomeTiles, BiomeType biome, vector<vector<Terrain>>& worldMap, int worldWidth, int worldHeight)
//{
//    std::queue<POINT> q;
//    std::vector<std::vector<bool>> visited(worldWidth, std::vector<bool>(worldHeight, false));
//    q.push(center);  // �߽� ��ǥ���� ����
//
//    int placedTiles = 0;
//    std::vector<std::pair<int, int>> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} }; // 4���� Ȯ��
//
//    // �߽ɿ��� ������ Ÿ�Ϸ� Ȯ���ϴ� BFS ����
//    while (!q.empty() && placedTiles < biomeTiles)
//    {
//        POINT current = q.front();
//        q.pop();
//
//        int intX = current.x;
//        int intY = current.y;
//
//        // ���� ��踦 ���� �ʵ��� ���� �˻� �߰�
//        if (intX >= 0 && intX < worldWidth && intY >= 0 && intY < worldHeight && !visited[intX][intY])
//        {
//            visited[intX][intY] = true;
//
//            // �̹� ���� �ƴ� Ÿ���� ��� �ǳʶ�
//            if (worldMap[intX][intY].getBiomeType() != BiomeType::Water)
//            {
//                continue;
//            }
//
//            // ���� ��쿡�� Ÿ�� ��ġ
//            worldMap[intX][intY].update(biome);
//            placedTiles++;
//
//            // ������ Ÿ���� ť�� �߰� (��� �������� �߰�)
//            for (const auto& dir : directions)
//            {
//                POINT nextTile = { intX + dir.first, intY + dir.second };
//                if (nextTile.x >= 0 && nextTile.x < worldWidth && nextTile.y >= 0 && nextTile.y < worldHeight && !visited[nextTile.x][nextTile.y])
//                {
//                    q.push(nextTile);
//                }
//            }
//        }
//    }
//
//    // Ȯ���� �� �ִ� Ÿ���� ������ ���� Ÿ���� ã�Ƽ� ��� Ȯ��
//    while (placedTiles < biomeTiles)
//    {
//        bool foundNewExpansion = false;
//        for (int x = std::max(0, (int)(center.x - 1)); x <= std::min(worldWidth - 1, (int)(center.x + 1)); ++x)
//        {
//            for (int y = std::max(0, (int)(center.y - 1)); y <= std::min(worldHeight - 1, (int)(center.y + 1)); ++y)
//            {
//                if (!visited[x][y] && worldMap[x][y].getBiomeType() == BiomeType::Water)
//                {
//                    // Ÿ���� Ȯ��
//                    worldMap[x][y].update(biome);
//                    placedTiles++;
//                    visited[x][y] = true;
//                    q.push({ x, y });
//                    foundNewExpansion = true;
//                    break;
//                }
//            }
//            if (foundNewExpansion) break;
//        }
//        if (!foundNewExpansion) break; // Ȯ���� Ÿ���� ������ ����
//    }
//}
//
//
//
//// ���̿Ⱥ� �߽���ǥ ����, MST�˰��� ����
//void World::generateWorld(POINT* wldCenterPos)
//{
//    // 8���� �ʼ� ���̿� ����
//    std::vector<BiomeType> biomes;
//
//    for (int i = 1; i < 9; ++i)
//    {
//        biomes.push_back(BiomeType(i));
//    }
//
//    // ���� ������ ���� 0~4���� ���̿� �߰�
//    int addBiomes = rand() % 5;
//    for (int i = 0; i < addBiomes; ++i)
//    {
//        int randomBiome = rand() % 7;
//        biomes.push_back((BiomeType)(randomBiome + 2));
//    }
//
//    // ���̿� ������ ����
//    for (int i = 0; i < biomes.size(); ++i)
//    {
//        int randomIndex = rand() % biomes.size();
//        std::swap(biomes[i], biomes[randomIndex]);
//    }
//
//    // �߽���ǥ
//    int centerX = wldCenterPos->x;
//    int centerY = wldCenterPos->y;
//
//    // ���̿ȴ� Ÿ�� �� ��� �� ����
//    float landPercentage = 0.7f;
//    int maxChangeTile = static_cast<int>(worldWidth * worldHeight * landPercentage);
//    vector<int> biomeTileTargets(biomes.size());
//    for (size_t i = 0; i < biomes.size(); ++i)
//    {
//        biomeTileTargets[i] = maxChangeTile / biomes.size();
//    }
//
//    // ���̿� �߽���ǥ ����
//    vector<POINT> biomeCenters;
//    int minDistance = min(worldWidth, worldHeight) * 0.1;
//
//    for (size_t i = 0; i < biomes.size(); ++i)
//    {
//        bool valid = false;
//        POINT newCenter;
//
//        while (!valid)
//        {
//            newCenter.x = rand() % worldWidth;
//            newCenter.y = rand() % worldHeight;
//            valid = true;
//
//            for (const auto& center : biomeCenters)
//            {
//                int distance = calcDist(center, newCenter);
//                if (distance < minDistance)
//                {
//                    valid = false;
//                    break;
//                }
//            }
//        }
//
//        biomeCenters.push_back(newCenter); // ��ȿ�� �߽� �߰�
//    }
//
//    // MST�� ����
//    vector<pair<int, int>> mstEdges = generateMST(biomeCenters);
//
//    // MST ��θ� ���� Ÿ�� ��ġ
//    for (const auto& edge : mstEdges)
//    {
//        int biome1 = edge.first;
//        int biome2 = edge.second;
//
//        POINT start = biomeCenters[biome1];
//        POINT end = biomeCenters[biome2];
//
//        int deltaX = end.x - start.x;
//        int deltaY = end.y - start.y;
//        int steps = max(abs(deltaX), abs(deltaY));
//
//        float xIncrement = deltaX / static_cast<float>(steps);
//        float yIncrement = deltaY / static_cast<float>(steps);
//
//        float currentX = start.x;
//        float currentY = start.y;
//
//        for (int j = 0; j <= steps; ++j)
//        {
//            int intX = static_cast<int>(currentX);
//            int intY = static_cast<int>(currentY);
//
//            if (intX >= 0 && intX < worldWidth && intY >= 0 && intY < worldHeight)
//            {
//                if (worldMap[intX][intY].getBiomeType() != BiomeType::Water)
//                {
//                    biomeTileTargets[biome1]--; // Ÿ���� �̹� ��ġ�� ���
//                    continue;
//                }
//
//                worldMap[intX][intY].update(biomes[biome1]);
//                ++cnt;
//            }
//
//            currentX += xIncrement;
//            currentY += yIncrement;
//        }
//    }
//
//    // �� ���̿� �߽� �ֺ��� Ÿ�� ��ġ (BFS)
//    for (size_t i = 0; i < biomes.size(); ++i)
//    {
//        bfsExpandBiome(biomeCenters[i], biomeTileTargets[i], biomes[i], worldMap, worldWidth, worldHeight);
//    }
//}
//
