#include "pch.h"
#include "UIMgr.h"
#include "UI.h"
#include "HUD.h"
#include "Map.h"
#include "Core.h"
#include "GameMgr.h"

UIMgr::UIMgr()
	: uis{}
	, focusedUI(nullptr)
	, canvas(Vec2D(), Vec2D(), nullptr)
	, canvasResolution(1920.f, 1080.f)
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

	for (size_t i = 0; i < uis.size(); ++i)
	{
		uis[i]->SetParent(&canvas);
	}
}

void UIMgr::update()
{
	POINT resolution = Core::GetInst()->GetResolution();
	Vec2D scale(resolution.x / canvasResolution.x, resolution.y / canvasResolution.y);
	if (scale.x != canvas.scale.x || scale.y != canvas.scale.y)
	{
		canvas.scale = scale;

		for (size_t i = 0; i < uis.size(); ++i)
		{
			uis[i]->Resize();
		}
	}

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
	bool isOpen = !uis[1]->IsOpen();
	GameMgr::GetInst()->SetPause(isOpen);
	uis[1]->SetOpen(isOpen);
	focusedUI = isOpen ? uis[1] : nullptr;
}

void UIMgr::Execute(const InputContext& ctx)
{
	if (focusedUI != nullptr)
	{
		focusedUI->Execute(ctx);
	}
}

void UIMgr::Refresh(const wstring& target)
{
	for (size_t i = 0; i < uis.size(); ++i)
	{
		if (uis[i]->GetName() == target)
		{
			uis[i]->Refresh();
			return;
		}
	}
}