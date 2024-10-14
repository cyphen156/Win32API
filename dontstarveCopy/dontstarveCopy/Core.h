#pragma once

class Core
{
	SINGLE(Core);

private:
	HWND	m_hWnd;
	RECT*	m_ptWinInfo;
	HDC		m_hdc;
	//float	fps;
	HBITMAP m_hBit;
	HDC		m_MemDC;

	void update();
	void render();

public:
	int init(HWND hWnd, RECT* windowInfo);
	void progress();
	HWND GetMainHWND();
};

