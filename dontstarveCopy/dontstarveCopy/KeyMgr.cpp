#include "pch.h"
#include "KeyMgr.h"
#include "Core.h"

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
	VK_RETURN,
	VK_SPACE,
	VK_ESCAPE,
	VK_TAB,
	VK_CAPITAL,		//CAPSLOCK

	VK_LBUTTON,
	VK_RBUTTON,
};

KeyMgr::KeyMgr() {}

KeyMgr::~KeyMgr() {}

void KeyMgr::init() 
{
	for (int i = 00; i < (int)KEY::LAST; ++i) 
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}

void KeyMgr::update()
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
}