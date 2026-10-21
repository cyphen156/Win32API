#include "pch.h"
#include "World.h"
#include "CameraMgr.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <functional>

#define PI 3.14159265358979323846

World::World(SceneType type)
    : wldSize(0, 0)
    , worldMap{}
	, tiles{}
    , tileSize(0)
    , biomeBrush{}
{
    const COLORREF biomeColors[(UINT)BiomeType::END] =
    {
        RGB(64, 120, 190), RGB(250, 199, 117), RGB(59, 109, 17), RGB(200, 180, 70), RGB(160, 95, 40),
        RGB(90, 80, 110), RGB(235, 160, 90), RGB(151, 196, 89), RGB(140, 140, 140),
    };
    for (UINT i = 0; i < (UINT)BiomeType::END; ++i)
    {
        biomeBrush[i] = CreateSolidBrush(biomeColors[i]);
    }
    // 랜덤 시드 초기화
    srand(static_cast<unsigned>(time(0)));
}

World::~World() 
{
    for (UINT i = 0; i < (UINT)BiomeType::END; ++i)
    {
        DeleteObject(biomeBrush[i]);
    }
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
    wstring resPath = PathMgr::GetInst()->GetContentPath();
    resPath += L"\\Textures\\Particle";
    
    LoadTile(resPath);
	setTileSize(120);
	// 전체 월드 생성, default Biome : water
    worldMap.resize(wldSize.x, std::vector<Terrain>(wldSize.y, BiomeType::Water));

    POINT wldCenterPos = { (LONG)(wldSize.x / 2), (LONG)(wldSize.y / 2) };
    generateWorld(&wldCenterPos);
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
    for (int tileY = firstY; tileY <= lastY; ++tileY)
    {
        for (int tileX = firstX; tileX <= lastX; ++tileX)
        {
            BiomeType biome = BiomeType::Water;
            if (tileX >= 0 && tileX < (int)worldMap.size() && tileY >= 0 && tileY < (int)worldMap[tileX].size())
            {
                biome = worldMap[tileX][tileY].getBiomeType();
            }
            Vec2D renderPos = camera->GetRenderPos(Vec2D(tileX * tileSize, tileY * tileSize));
            RECT tileRect = { (LONG)floor(renderPos.x), (LONG)floor(renderPos.y),
                (LONG)ceil(renderPos.x + tileSize * zoom), (LONG)ceil(renderPos.y + tileSize * groundZoom) };
            FillRect(hdc, &tileRect, biomeBrush[(UINT)biome]);
        }
    }
}

void World::LoadTile(wstring& TexPath)
{
    WIN32_FIND_DATA findDir;
	HANDLE hFind = FindFirstFile((TexPath + L"\\*").c_str(), &findDir);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		// 로더 경로명 문제 발생
		MessageBox(nullptr, (L"Cannot find Dir" + TexPath + L" Error :: Invalid Path.").c_str(), L"Error", MB_OK);
		return;
	}
	
	bool isEmpty = true;

	do {
		if (wcscmp(findDir.cFileName, L".") != 0 && wcscmp(findDir.cFileName, L"..") != 0)
		{
			isEmpty = false;
			wstring fullPath = TexPath + L"\\" + findDir.cFileName;

			if (findDir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				LoadTile(fullPath);
			}
			else if (wstring(findDir.cFileName).find(L".png") != wstring::npos)
			{
				// 하위 폴더가 없고, png파일이 있다면
				wstring fileName = findDir.cFileName;

				Texture* pTex = ResourceMgr::GetInst()->LoadTexture(fileName, fullPath);
				if (pTex)
				{
					tiles.push_back(pTex);
					//MessageBox(nullptr, (fileName + L" is loaded " + std::to_wstring(frames.size()) + L" frames").c_str(), L"Notice", MB_OK);
				}
			}
		}
	} while (FindNextFile(hFind, &findDir));

	if (isEmpty)
	{
		// 폴더는 있지만 하위에 파일이나 폴더가 없는 경우
		MessageBox(nullptr, (L"Find " + TexPath + L"but it is Empty Dir.").c_str(), L"Notice", MB_OK);
	}
	FindClose(hFind);
}
int calcDist(const POINT& a, const POINT& b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Prim 알고리즘 MST
vector<pair<int, int>> generateMST(const vector<POINT>& biomeCenters)
{
    int biomecnt = biomeCenters.size();
    vector<bool> isInMST(biomecnt, false);
    vector<int> minDistance(biomecnt, INT_MAX);
    vector<int> prev(biomecnt, -1);
    vector<pair<int, int>> mstEdges;

    minDistance[0] = 0;

    for (int iter = 0; iter < biomecnt; ++iter)
    {
        int currBiome = -1;
        for (int i = 0; i < biomecnt; ++i)
        {
            if (!isInMST[i] && (currBiome == -1 || minDistance[i] < minDistance[currBiome]))
            {
                currBiome = i;
            }
        }

        isInMST[currBiome] = true;

        if (prev[currBiome] != -1)
        {
            mstEdges.push_back({ prev[currBiome], currBiome });
        }

        // 거리 갱신
        for (int next = 0; next < biomecnt; ++next)
        {
            if (!isInMST[next])
            {
                int distance = calcDist(biomeCenters[currBiome], biomeCenters[next]);
                if (distance < minDistance[next])
                {
                    minDistance[next] = distance;
                    prev[next] = currBiome;
                }
            }
        }
    }

    return mstEdges;
}

void bfsExpandBiome(const POINT& center, int& biomeTiles, BiomeType biome, vector<vector<Terrain>>& worldMap, int worldWidth, int worldHeight)
{
    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, std::greater<std::pair<int, std::pair<int, int>>>> q;
    std::vector<std::vector<bool>> visited(worldWidth, std::vector<bool>(worldHeight, false));
    q.push({ 0, { center.x, center.y } });  // 중심 좌표에서 시작

    int placedTiles = 0;
    std::vector<std::pair<int, int>> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} }; // 4방향 확장

    // 중심에서 인접한 타일로 확장하는 BFS 루프
    while (!q.empty() && placedTiles < biomeTiles)
    {
        POINT current = { q.top().second.first, q.top().second.second };
        q.pop();

        int intX = current.x;
        int intY = current.y;

        // 월드 경계를 넘지 않도록 범위 검사 추가
        if (intX >= 0 && intX < worldWidth && intY >= 0 && intY < worldHeight && !visited[intX][intY])
        {
            visited[intX][intY] = true;

            // 다른 바이옴 타일일 경우 건너뜀
            if (worldMap[intX][intY].getBiomeType() != BiomeType::Water && worldMap[intX][intY].getBiomeType() != biome)
            {
                continue;
            }

            // 물일 경우에만 타일 배치
            if (worldMap[intX][intY].getBiomeType() == BiomeType::Water)
            {
                worldMap[intX][intY].update(biome);
                placedTiles++;
            }

            // 인접한 타일을 큐에 추가 (경계 내에서만 추가)
            for (const auto& dir : directions)
            {
                POINT nextTile = { intX + dir.first, intY + dir.second };
                if (nextTile.x >= 0 && nextTile.x < worldWidth && nextTile.y >= 0 && nextTile.y < worldHeight && !visited[nextTile.x][nextTile.y])
                {
                    int squaredDistance = (nextTile.x - center.x) * (nextTile.x - center.x) + (nextTile.y - center.y) * (nextTile.y - center.y);
                    q.push({ squaredDistance, { nextTile.x, nextTile.y } });
                }
            }
        }
    }
}

// 바이옴별 중심좌표 생성
void World::generateWorld(POINT* wldCenterPos)
{
    int worldWidth = (int)wldSize.x;
    int worldHeight = (int)wldSize.y;

    // 8개의 필수 바이옴 생성
    std::vector<BiomeType> biomes;

    for (int i = 1; i < 9; ++i)
    {
        biomes.push_back(BiomeType(i));
    }

    // 중심좌표
    int centerX = wldCenterPos->x;
    int centerY = wldCenterPos->y;

    // 바이옴당 타일 수 계산 및 조정
    float landPercentage = 0.35f;
    int maxChangeTile = static_cast<int>(worldWidth * worldHeight * landPercentage);
    vector<int> biomeTileTargets(biomes.size());
    for (size_t i = 0; i < biomes.size(); ++i)
    {
        biomeTileTargets[i] = (maxChangeTile + biomes.size() - 1) / biomes.size();
    }

    // 바이옴 중심좌표 생성
    vector<POINT> biomeCenters;
    double spawnRadius = sqrt(biomeTileTargets[0] / PI);

    for (size_t i = 0; i < biomes.size(); ++i)
    {
        POINT newCenter = { centerX, centerY };

        if (i > 0)
        {
            double angle = (i - 1) * 2 * PI / (biomes.size() - 1);
            double distance = spawnRadius + sqrt(biomeTileTargets[i] / PI);
            newCenter.x = centerX + lround(cos(angle) * distance);
            newCenter.y = centerY + lround(sin(angle) * distance);
        }

        biomeCenters.push_back(newCenter); // 유효한 중심 추가
    }

    bfsExpandBiome(biomeCenters[0], biomeTileTargets[0], biomes[0], worldMap, worldWidth, worldHeight);

    // 경로를 따라 타일 배치
    for (size_t i = 1; i < biomes.size(); ++i)
    {
        POINT start = biomeCenters[i];
        POINT end = biomeCenters[0];

        int deltaX = end.x - start.x;
        int deltaY = end.y - start.y;
        int steps = max(abs(deltaX), abs(deltaY));

        float xIncrement = deltaX / static_cast<float>(steps);
        float yIncrement = deltaY / static_cast<float>(steps);

        float currentX = start.x;
        float currentY = start.y;

        for (int j = 0; j <= steps; ++j)
        {
            int intX = static_cast<int>(currentX);
            int intY = static_cast<int>(currentY);

            if (intX >= 0 && intX < worldWidth && intY >= 0 && intY < worldHeight)
            {
                if (worldMap[intX][intY].getBiomeType() != BiomeType::Water)
                {
                    break;
                }

                worldMap[intX][intY].update(biomes[i]);
                biomeTileTargets[i]--;
            }

            currentX += xIncrement;
            currentY += yIncrement;
        }
    }

    // 각 바이옴 중심 주변에 타일 배치 (BFS)
    for (size_t i = 1; i < biomes.size(); ++i)
    {
        bfsExpandBiome(biomeCenters[i], biomeTileTargets[i], biomes[i], worldMap, worldWidth, worldHeight);
    }
}
