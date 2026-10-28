#include "pch.h"
#include "Tile.h"

TileDef tileDefs[TILEDEFCOUNT] =
{
	{ TileType::OceanCoastal, L"Default", 1 },
	{ TileType::OceanSwell, L"Default", 1 },
	{ TileType::OceanRough, L"Default", 1 },
	{ TileType::OceanHazardous, L"Default", 1 },
	{ TileType::Marsh, L"Default", 48 },
	{ TileType::Rocky, L"Default", 48 },
	{ TileType::Savanna, L"Default", 48 },
	{ TileType::Forest, L"Default", 48 },
	{ TileType::Grass, L"Default", 48 },
	{ TileType::Dirt, L"Default", 48 },
	{ TileType::Deciduous, L"Default", 48 },
	{ TileType::DesertDirt, L"Default", 48 },
	{ TileType::Forest, L"VariantA", 16 },
	{ TileType::Forest, L"VariantB", 16 },
	{ TileType::Forest, L"VariantC", 1 },
	{ TileType::Forest, L"VariantD", 1 },
	{ TileType::Forest, L"VariantE", 1 },
	{ TileType::Grass, L"VariantA", 16 },
	{ TileType::Grass, L"VariantB", 16 },
};