#include "pch.h"
#include "Tile.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "Texture.h"

Tile tileDefs[(UINT)TileType::END] =
{
	{ TileType::OceanCoastal, L"OceanCoastal", L"Default", 0, 0, 0 },
	{ TileType::OceanSwell, L"OceanSwell", L"Default", 0, 0, 0 },
	{ TileType::OceanRough, L"OceanRough", L"Default", 0, 0, 0 },
	{ TileType::OceanHazardous, L"OceanHazardous", L"Default", 0, 0, 0 },
	{ TileType::Marsh, L"Marsh", L"Default", 0, 47, 0 },
	{ TileType::Rocky, L"Rocky", L"Default", 0, 47, 0 },
	{ TileType::Savanna, L"YellowGrass", L"Default", 0, 47, 0 },
	{ TileType::Forest, L"Forest", L"Default", 0, 47, 0 },
	{ TileType::Grass, L"Grass", L"Default", 0, 47, 0 },
	{ TileType::Dirt, L"Dirt", L"Default", 0, 47, 0 },
	{ TileType::Deciduous, L"Deciduous", L"Default", 0, 47, 0 },
	{ TileType::DesertDirt, L"DesertDirt", L"Default", 0, 47, 0 },
};

// 타일 그리기. 텍스처는 처음 그릴 때 읽는다
void Tile::render(HDC hdc, const RECT& tileRect)
{
    if (texture == nullptr)
    {
        wchar_t drawIndexName[8] = {};
        swprintf_s(drawIndexName, L"%03d", drawIndex);
        wstring tileName = groundName + L"_" + setName + L"_" + drawIndexName + L".png";
        wstring tilePath = PathMgr::GetInst()->GetContentPath();
        tilePath += L"\\Textures\\Terrain\\" + groundName + L"\\" + setName + L"\\" + tileName;
        texture = ResourceMgr::GetInst()->LoadTexture(tileName, tilePath);
    }
    HDC tileDC = texture->GetDC();
    StretchBlt(hdc, tileRect.left, tileRect.top, tileRect.right - tileRect.left, tileRect.bottom - tileRect.top,
        tileDC, 0, 0, texture->Width(), texture->Height(), SRCCOPY);
}

// 지도 무늬. 처음 부를 때 읽는다
Texture* Tile::getMapTexture()
{
    if (mapTexture == nullptr)
    {
        wstring mapName = L"Map_" + groundName + L"_000.png";
        wstring mapPath = PathMgr::GetInst()->GetContentPath();
        mapPath += L"\\Textures\\UI\\Map\\" + groundName + L"\\" + mapName;
        mapTexture = ResourceMgr::GetInst()->LoadTexture(mapName, mapPath);
    }
    return mapTexture;
}
