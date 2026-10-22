#include "pch.h"
#include "HUD.h"

HUD::HUD()
{
	SetOpen(true);
}

HUD::~HUD()
{
}

void HUD::update()
{
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
