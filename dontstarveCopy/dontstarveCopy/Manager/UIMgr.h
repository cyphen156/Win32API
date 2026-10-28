#pragma once

class UI;

class UIMgr
{
	SINGLE(UIMgr);

private:
	vector<UI*> uis;

public:
	void init();
	void update();
	void render(HDC hdc);

	void ToggleMap();
};
