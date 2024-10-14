#include "pch.h"
#include "Core.h"
#include "Object.h"
#include "TimeMgr.h"
#include "PathMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

Core::Core() : m_hWnd(0), m_ptWinInfo(nullptr), m_hdc(0), m_hBit(0), m_MemDC(0) {};

Core::~Core() 
{
	ReleaseDC(m_hWnd, m_hdc);

	DeleteDC(m_MemDC);
	DeleteObject(m_hBit);
};

int Core::init(HWND hWnd, RECT* windowInfo)
{
	m_hWnd = hWnd;
	m_ptWinInfo = windowInfo;
	
	m_hdc = GetDC(m_hWnd);
	
	// �Ŵ��� �ʱ�ȭ
	TimeMgr::GetInst()->init();
	PathMgr::GetInst()->init();
	KeyMgr::GetInst()->init();
	SceneMgr::GetInst()->init();

	// ��Ʈ�� ����
	m_hBit = CreateCompatibleBitmap(m_hdc, (int)(m_ptWinInfo->right - m_ptWinInfo->left), (int)(m_ptWinInfo->bottom - m_ptWinInfo->top));
	m_MemDC = CreateCompatibleDC(m_hdc);

	

	return S_OK;
}
void Core::progress()
{
	// �ʴ� ������ ����ȭ
	TimeMgr::GetInst()->update();
	KeyMgr::GetInst()->update();
	SceneMgr::GetInst()->update();

	update();

	render();

}
HWND Core::GetMainHWND()
{
	return m_hWnd;
};

void Core::update()
{
	// �÷��̾� ������

	// ���� ������Ʈ

	// ����� ������Ʈ
}

void Core::render()
{
	// ����� �ؽ��� ������

	// �÷��̾� ������

	// ���� ������
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_MemDC, m_hBit);

	Rectangle(m_MemDC, 0, 0, m_ptWinInfo->right, m_ptWinInfo->bottom);
	// �� ���ۿ��� ���� DC�� �׸���
	BitBlt(m_hdc, 0, 0, (int)m_ptWinInfo->right, (int)m_ptWinInfo->bottom, m_MemDC, 0, 0, SRCCOPY);
	
	DeleteObject(hOldBit);
	
}