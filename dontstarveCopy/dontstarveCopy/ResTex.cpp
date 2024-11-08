#include "pch.h"
#include "ResTex.h"
#pragma comment(lib, "Msimg32.lib") 

ResTex::ResTex() : m_hBit(0), m_dc(0), m_bitInfo{}, m_img(nullptr)
{
}

ResTex::~ResTex()
{
	ReleaseImage();
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

void ResTex::Load(const wstring& resPath)
{
    m_img = Image::FromFile(resPath.c_str());
    if (m_img->GetLastStatus() != Ok) {
        MessageBox(nullptr, L"�̹��� �ε� ����!", L"Error", MB_OK);
        delete m_img; // �ε� ���� �� �޸� ����
        m_img = nullptr;
        return;
    }
    //int width = m_img->GetWidth();
    //int height = m_img->GetHeight();

    //Bitmap* bitmap = new Bitmap(width, height, PixelFormat32bppRGB);
    //Graphics graphics(bitmap);

    //// ���� �̹����� ��Ʈ������ �׸���
    //graphics.DrawImage(m_img, 0, 0, width, height);

    //bitmap->GetHBITMAP(Color(0, 0, 0, 0), &m_hBit);
    //delete bitmap;

    //MessageBox(nullptr, L"�̹��� �ε� ����!", L"Error", MB_OK);
	//m_hBit = (HBITMAP)m_img->GetHBITMAP(Color(0, 0, 0, 0));
	//m_hBit = (HBITMAP)LoadImage(nullptr, resPath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	//assert(m_hBit);
}

void ResTex::ReleaseImage() {
	if (m_img) {
		delete m_img;
		m_img = nullptr;
	}
}