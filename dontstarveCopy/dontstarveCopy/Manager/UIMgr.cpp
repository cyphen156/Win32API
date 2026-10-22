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
	// 위에 그려지는 UI가 입력을 먼저 받는다
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
