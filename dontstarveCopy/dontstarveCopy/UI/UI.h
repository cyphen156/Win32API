#pragma once

// 씬 위에 그리는 화면 하나
class UI
{
protected:
	bool isOpen;

public:
	UI();
	virtual ~UI();

	virtual void update() = 0;
	virtual void render(HDC hdc) = 0;

	bool IsOpen() { return isOpen; }
	void SetOpen(bool open) { isOpen = open; }
};
