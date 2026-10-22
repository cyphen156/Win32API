#include "pch.h"
#include "Texture.h"
#pragma comment(lib, "Msimg32.lib") 

Texture::Texture() : m_hBit(0), m_dc(0), m_bitInfo{}, m_img(nullptr)
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

void Texture::ReleaseImage() {
	if (m_img) {
		delete m_img;
		m_img = nullptr;
	}
}