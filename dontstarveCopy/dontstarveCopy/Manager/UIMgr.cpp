#include "pch.h"
#include "UIMgr.h"
#include "UI.h"
#include "HUD.h"
#include "Map.h"

UIMgr::UIMgr()
	: uis{}
{
}

UIMgr::~UIMgr()
{
	Safe_Delete_Vec<UI*>(uis);
}

void UIMgr::init()
{
	uis.push_back(new HUD);
	uis.push_back(new Map);
}

void UIMgr::update()
{
	for (int i = (int)uis.size() - 1; i >= 0; --i)
	{
		uis[i]->update();
	}
}

void UIMgr::render(HDC hdc)
{
	for (size_t i = 0; i < uis.size(); ++i)
	{
		if (uis[i]->IsOpen())
		{
			uis[i]->render(hdc);
		}
	}
}

void UIMgr::ToggleMap()
{
	uis[1]->SetOpen(!uis[1]->IsOpen());
}
