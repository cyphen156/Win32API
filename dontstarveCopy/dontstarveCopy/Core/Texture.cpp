#include "pch.h"
#include "Texture.h"
#pragma comment(lib, "Msimg32.lib") 

Texture::Texture() : m_hBit(0), m_dc(0), m_bitInfo{}, m_img(nullptr), m_opaqueBounds{}
{
}

Texture::~Texture()
{
	ReleaseImage();
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

void Texture::Load(const wstring& resPath)
{
    m_img = Image::FromFile(resPath.c_str());
    if (m_img->GetLastStatus() != Ok) {
        MessageBox(nullptr, L"이미지 로드 실패!", L"Error", MB_OK);
        delete m_img; // 로드 실패 시 메모리 해제
        m_img = nullptr;
        return;
    }
}

HDC Texture::GetDC()
{
	if (m_dc == nullptr && m_img != nullptr)
	{
		Bitmap bitmap(m_img->GetWidth(), m_img->GetHeight(), PixelFormat32bppRGB);
		Graphics graphics(&bitmap);
		graphics.DrawImage(m_img, 0, 0, m_img->GetWidth(), m_img->GetHeight());
		bitmap.GetHBITMAP(Color(0, 0, 0, 0), &m_hBit);
		m_dc = CreateCompatibleDC(nullptr);
		SelectObject(m_dc, m_hBit);
		GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
	}
	return m_dc;
}

const RECT& Texture::GetOpaqueBounds()
{
	if (m_opaqueBounds.right == 0 && m_img != nullptr)
	{
		Bitmap* bitmap = (Bitmap*)m_img;
		int width = (int)bitmap->GetWidth(), height = (int)bitmap->GetHeight();
		Rect whole(0, 0, width, height);
		BitmapData data = {};
		int minX = width, minY = height, maxX = -1, maxY = -1;
		if (bitmap->LockBits(&whole, ImageLockModeRead, PixelFormat32bppARGB, &data) == Ok)
		{
			for (int y = 0; y < height; ++y)
			{
				const BYTE* row = (const BYTE*)data.Scan0 + (size_t)y * data.Stride;
				for (int x = 0; x < width; ++x)
				{
					if (row[x * 4 + 3] != 0)
					{
						if (x < minX) minX = x;
						if (x > maxX) maxX = x;
						if (y < minY) minY = y;
						if (y > maxY) maxY = y;
					}
				}
			}
			bitmap->UnlockBits(&data);
		}
		if (maxX < 0)
		{
			m_opaqueBounds = { 0, 0, width, height };
		}
		else
		{
			m_opaqueBounds = { max(minX - 1, 0), max(minY - 1, 0), min(maxX + 2, width), min(maxY + 2, height) };
		}
	}
	return m_opaqueBounds;
}

void Texture::ReleaseImage() {
	if (m_img) {
		delete m_img;
		m_img = nullptr;
	}
}