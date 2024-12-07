#include "pch.h"
#include "func2.h"

wstring func2::Enum_To_Wstring(SceneType type)
{
	switch (type) {
	case SceneType::START: return L"START";
	case SceneType::CAVE:  return L"CAVE";

	case SceneType::END:   return L"END";
	default:               return L"END";
	}
}
