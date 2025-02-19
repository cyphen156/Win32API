#pragma once
#include "Res.h"
#include "gdiplus.h"
#include <cassert>


class ResTex : public Res
{
private:
	HDC			m_dc;
	HBITMAP		m_hBit;
	BITMAP		m_bitInfo;
	Image*		m_img;
	ResTex();
	virtual ~ResTex();

public:
	UINT Width() { return m_bitInfo.bmWidth; }
	UINT Height() { return m_bitInfo.bmHeight; }

	HDC GetDC() { return m_dc; }
	Image* GetImage() { return m_img; }
	void Load(const wstring& resPath);

	void ReleaseImage();

	friend class ResMgr;
};
