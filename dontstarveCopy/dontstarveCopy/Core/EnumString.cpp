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

	case TileType::Cave:           return L"Cave";
	case TileType::Underrock:      return L"Underrock";
	case TileType::Fungus:         return L"Fungus";
	case TileType::FungusRed:      return L"FungusRed";
	case TileType::FungusGreen:    return L"FungusGreen";
	case TileType::Sinkhole:       return L"Sinkhole";
	case TileType::Mud:            return L"Mud";

	case TileType::END:            return L"END";
	default:                       return L"END";
	}	
}

wstring EnumString::ToWString(MonsterType type)
{
	switch (type)
	{
	case MonsterType::Pigman: return L"Pigman";
	case MonsterType::Spider: return L"Spider";

	case MonsterType::END:    return L"END";
	default:                 return L"END";
	}
}
