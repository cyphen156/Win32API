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
