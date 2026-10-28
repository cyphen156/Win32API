#include "pch.h"
#include "EnumString.h"

wstring EnumString::ToWString(SceneType type)
{
	switch (type) 
	{
		case SceneType::START: return L"START";
		case SceneType::CAVE:  return L"CAVE";

		case SceneType::END:   return L"END";
		default:               return L"END";
	}
}

wstring EnumString::ToWString(TileType type)
{
	switch (type) 
	{
	case TileType::OceanCoastal:   return L"OceanCoastal";
	case TileType::OceanSwell:     return L"OceanSwell";
	case TileType::OceanRough:     return L"OceanRough";
	case TileType::OceanHazardous: return L"OceanHazardous";
	case TileType::Marsh:          return L"Marsh";
	case TileType::Rocky:          return L"Rocky";
	case TileType::Savanna:        return L"Savanna";
	case TileType::Forest:         return L"Forest";
	case TileType::Grass:          return L"Grass";
	case TileType::Dirt:           return L"Dirt";
	case TileType::Deciduous:      return L"Deciduous";
	case TileType::DesertDirt:     return L"DesertDirt";

	case TileType::END:            return L"END";
	default:                       return L"END";
	}
}