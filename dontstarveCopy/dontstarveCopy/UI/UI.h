#pragma once
#include "Transform.h"

class UI
{
protected:
	Transform transform;
	bool isOpen;

	virtual void OnOpen() {}
	virtual void OnClose() {}

public:
	UI();
	virtual ~UI();

	virtual void update() = 0;
	virtual void render(HDC hdc) = 0;

	bool IsOpen() { return isOpen; }
	void SetOpen(bool open);
};
