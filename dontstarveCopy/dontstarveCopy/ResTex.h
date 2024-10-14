#pragma once
#include "ResMgr.h"
#include "gdiplus.h"
#include <cassert>

class ResTex : public ResMgr
{
private:
	HDC			m_dc;
	HBITMAP		m_hBit;

public:
	ResTex();
	~ResTex();

	void Load(const wstring& resPath);
};
