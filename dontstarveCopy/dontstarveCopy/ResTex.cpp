#include "pch.h"
#include "ResTex.h"
#pragma comment(lib, "Msimg32.lib") 

ResTex::ResTex() : m_hBit(0), m_dc(0), m_bitInfo{}
{
}

ResTex::~ResTex()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

void ResTex::Load(const wstring& resPath)
{
	m_hBit = (HBITMAP)LoadImage(nullptr, resPath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	//assert(m_hBit);
};

