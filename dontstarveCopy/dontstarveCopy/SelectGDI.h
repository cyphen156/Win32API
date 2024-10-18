#pragma once
class SelectGDI
{
private:
	HDC		m_hdc;
	HPEN	m_hDefaultPen;
	HBRUSH	m_hDefaultBrush;

public:
	SelectGDI(HDC hdc, PEN_TYPE	type);
	SelectGDI(HDC hdc, BRUSH_TYPE type);
	~SelectGDI();
};

