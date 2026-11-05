#pragma once
#include "Core/Resource.h"
#include "gdiplus.h"
#include <cassert>


class Texture : public Resource
{
private:
	HDC			m_dc;
	HBITMAP		m_hBit;
	BITMAP		m_bitInfo;
	Image*		m_img;
	RECT		m_opaqueBounds;
	Texture();
	virtual ~Texture();

public:
	UINT Width() { return m_bitInfo.bmWidth; }
	UINT Height() { return m_bitInfo.bmHeight; }

	HDC GetDC();
	Image* GetImage() { return m_img; }
	const RECT& GetOpaqueBounds();
	void Load(const wstring& resPath);

	void ReleaseImage();

	friend class ResourceMgr;
};
