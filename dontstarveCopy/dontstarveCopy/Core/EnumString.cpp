#include "pch.h"
#include "EnumString.h"

wstring EnumString::ToWString(SceneType type)
{
	switch (type) {
	case SceneType::START: return L"START";
	case SceneType::CAVE:  return L"CAVE";

	case SceneType::END:   return L"END";
	default:               return L"END";
	}
}
