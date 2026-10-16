#include "pch.h"
#include "SelectGDI.h"
#include "Core.h"

SelectGDI::SelectGDI(HDC hdc, PEN_TYPE type)
	: m_hdc(hdc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HPEN hNewPen = Core::GetInst()->GetPen(type);
	HPEN hDefaultPen = (HPEN)SelectObject(hdc, hNewPen);
}

SelectGDI::SelectGDI(HDC hdc, BRUSH_TYPE type)
	: m_hdc(hdc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HBRUSH hNewBrush = Core::GetInst()->GetBrush(type);
	HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_hdc, m_hDefaultPen);
	SelectObject(m_hdc, m_hDefaultBrush);
}
