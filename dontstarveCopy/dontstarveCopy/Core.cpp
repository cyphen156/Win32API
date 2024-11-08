#include "pch.h"
#include "Core.h"
#include "Object.h"
#include "TimeMgr.h"
#include "PathMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "ResMgr.h"
#include "CollisionMgr.h"
#include "EventMgr.h"


Core::Core()
	: m_hWnd(0)
	, m_ptWinInfo(nullptr)
	, m_hdc(0)
	, m_hBit(0)
	, m_MemDC(0)
	, m_arrBrush{}
	, m_arrPen{}
{};

Core::~Core() 
{
	GdiplusShutdown(gdiplusToken);
	ReleaseDC(m_hWnd, m_hdc);

	DeleteDC(m_MemDC);
	DeleteObject(m_hBit);

	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
};

int Core::init(HWND hWnd, RECT* windowInfo)
{
	m_hWnd = hWnd;
	m_ptWinInfo = windowInfo;

	GdiplusStartupInput gdiplusStartupInput;
	if (::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Ok) 
	{
		MessageBox(nullptr, L"GDI+ Initialization failed!", L"Error", MB_OK);
		return E_FAIL; // �ʱ�ȭ ����
	}

	m_hdc = GetDC(m_hWnd);
	
	/*Graphics graphics(m_hdc);

	Image* image = Image::FromFile(L"D:\\project\\Win32API\\dontstarveCopy\\Output\\bin\\content\\Textures\\Player\\Idle\\Down\\Player_IDLE_DOWN_000.png");
	if (image->GetLastStatus() != Ok)
	{
		MessageBox(nullptr, L"Failed to load image.", L"Error", MB_OK);
	}

	graphics.DrawImage(image, 100, 100);*/


	// �⺻ GDI �ν��Ͻ� ����
	CreateBrushPen();
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
	update();

	render();
	

	// event ó��
	EventMgr::GetInst()->update();
}

POINT Core::GetReslution()
{
    POINT resolution;
    resolution.x = m_ptWinInfo->right;
    resolution.y = m_ptWinInfo->bottom;
    return resolution;
}
;

void Core::CreateBrushPen()
{
	// brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	// pen
	m_arrPen[(UINT)PEN_TYPE::RED] = (HPEN)CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEM] = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

}

void Core::update()
{
	// �ʴ� ������ ����ȭ
	TimeMgr::GetInst()->update();

	KeyMgr::GetInst()->update();
	// �� ������Ʈ
	SceneMgr::GetInst()->update();
	// �浹ü ������Ʈ
	CollisionMgr::GetInst()->update();
}

void Core::render()
{
	TimeMgr::GetInst()->render();

	// �� ������
	SceneMgr::GetInst()->render(m_hdc);
	// ����� �ؽ��� ������

	// �÷��̾� ������
	
	// ���� ������
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_MemDC, m_hBit);

	Rectangle(m_MemDC, 0, 0, m_ptWinInfo->right, m_ptWinInfo->bottom);
	// �� ���ۿ��� ���� DC�� �׸���
	BitBlt(m_hdc, 0, 0, (int)m_ptWinInfo->right, (int)m_ptWinInfo->bottom, m_MemDC, 0, 0, SRCCOPY);
	
	DeleteObject(hOldBit);
	
}

