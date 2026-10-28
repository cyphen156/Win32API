#include "pch.h"
#include "HUD.h"
#include "Core.h"

HUD::HUD()
{
	SetOpen(true);
}

HUD::~HUD()
{
}

void HUD::update()
{
	transform.scale = Vec2D(Core::GetInst()->GetResolution());

	if (minimap.IsOpen())
	{
		minimap.update();
	}
}

void HUD::render(HDC hdc)
{
	if (minimap.IsOpen())
	{
		minimap.render(hdc);
	}
}
