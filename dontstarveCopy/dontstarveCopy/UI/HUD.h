#pragma once
#include "UI.h"
#include "Minimap.h"

// 게임 화면 위에 늘 떠 있는 정보. 멈춤을 요청하지 않는다
class HUD : public UI
{
private:
	Minimap minimap;

public:
	HUD();
	~HUD();

	virtual void update();
	virtual void render(HDC hdc);
};
