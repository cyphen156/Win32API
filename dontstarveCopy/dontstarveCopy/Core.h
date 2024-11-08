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

	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

	void update();
	void render();

	void CreateBrushPen();

	ULONG_PTR gdiplusToken;
	Graphics* graphics;
public:

	int init(HWND hWnd, RECT* windowInfo);
	void progress();
	HWND GetMainHWND() { return m_hWnd; }
	HDC GetMainDC() { return m_hdc; }
	POINT GetReslution();
	HBRUSH GetBrush(BRUSH_TYPE type) { return m_arrBrush[(UINT)type]; }
	HPEN GetPen(PEN_TYPE type) { return m_arrPen[(UINT)type]; }
};

