#include "pch.h"
#include "UIMgr.h"
#include "UI.h"
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
	uis.push_back(new Map);
}

void UIMgr::update()
{
	for (size_t i = 0; i < uis.size(); ++i)
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
