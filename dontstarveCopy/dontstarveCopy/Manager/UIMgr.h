#pragma once
#include "Transform.h"

class UI;
struct InputContext;

class UIMgr
{
	SINGLE(UIMgr);

private:
	vector<UI*> uis;
	UI* focusedUI;
	Transform canvas;
	Vec2D canvasResolution;

public:
	void init();
	void update();
	void render(HDC hdc);

	void ToggleMap();

	void Execute(const InputContext& ctx);	
	void Refresh(const wstring& target);
	const Transform& GetCanvas() { return canvas; }
};