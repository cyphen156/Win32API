#include "pch.h"
#include "InputMgr.h"
#include "Core.h"
#include "UIMgr.h"

int g_arrVK[(int)KEY::LAST] = 
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	VK_LCONTROL,
	VK_LMENU,		//ALT
	VK_LSHIFT,
	VK_RETURN,		//ENTER
	VK_SPACE,
	VK_ESCAPE,
	VK_TAB,
	VK_CAPITAL,		//CAPSLOCK

	VK_LBUTTON,
	VK_RBUTTON,
};

InputMgr::InputMgr() : mousePos{}, mouseDelta{}, wheelInput(0), mouseWheel(0) {}

InputMgr::~InputMgr() {}

void InputMgr::init() 
{
	for (int i = 00; i < (int)KEY::LAST; ++i) 
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}

void InputMgr::update()
{
	HWND hMainHwnd = Core::GetInst()->GetMainHWND();
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		for (int i = 00; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌려 있다.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrev)
				{
					m_vecKey[i].eState = KEY_STATE::PRESS;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::KEYDOWN;
				}
				m_vecKey[i].bPrev = true;
			}
			else
			{
				if (m_vecKey[i].bPrev)
				{
					m_vecKey[i].eState = KEY_STATE::KEYUP;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrev = false;
			}
		}
	}

	else
	{
		for (int i = 00; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrev = false;

			if (m_vecKey[i].eState == KEY_STATE::KEYDOWN || m_vecKey[i].eState == KEY_STATE::PRESS)
			{
				m_vecKey[i].eState = KEY_STATE::KEYUP;
			}
			else if (m_vecKey[i].eState == KEY_STATE::KEYUP)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}

	// 마우스 위치는 메인 창 기준 좌표로 둔다
	POINT cursor = {};
	GetCursorPos(&cursor);
	ScreenToClient(hMainHwnd, &cursor);
	Vec2D nextMousePos(cursor);
	mouseDelta = nextMousePos - mousePos;
	mousePos = nextMousePos;

	// 휠은 창 메시지로 들어온 만큼 모았다가 이번 프레임 값으로 넘긴다
	mouseWheel = wheelInput;
	wheelInput = 0;

	if (m_vecKey[(int)KEY::TAB].eState == KEY_STATE::KEYDOWN)
	{
		UIMgr::GetInst()->ToggleMap();
	}
}