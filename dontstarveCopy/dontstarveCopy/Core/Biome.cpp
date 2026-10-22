#include "pch.h"
#include "Biome.h"

const Biome biomeDefs[(UINT)BiomeType::END] =
{
	{ BiomeType::Water, { TileType::OceanCoastal, TileType::OceanSwell, TileType::OceanRough, TileType::OceanHazardous } },
	{ BiomeType::Grass, { TileType::Grass } },
	{ BiomeType::Forest, { TileType::Forest } },
	{ BiomeType::Savanna, { TileType::Savanna } },
	{ BiomeType::Deciduous, { TileType::Deciduous } },
	{ BiomeType::Marsh, { TileType::Marsh } },
	{ BiomeType::Desert, { TileType::DesertDirt, TileType::Dirt } },
	{ BiomeType::MandrakeForest, { TileType::Forest, TileType::Savanna, TileType::Grass } },
	{ BiomeType::Rocky, { TileType::Rocky, TileType::Dirt } },
};
