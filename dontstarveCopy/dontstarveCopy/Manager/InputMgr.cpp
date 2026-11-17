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

InputMgr::InputMgr() 
	: context{}
	, wheelInput(0) 
{}

InputMgr::~InputMgr() {}

void InputMgr::init() 
{
	for (int i = 00; i < (int)KEY::LAST; ++i) 
	{
		context.keys.push_back(tKeyInfo{ KEY_STATE::NONE, false });
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
				if (context.keys[i].bPrev)
				{
					context.keys[i].eState = KEY_STATE::PRESS;
				}
				else
				{
					context.keys[i].eState = KEY_STATE::KEYDOWN;
				}
				context.keys[i].bPrev = true;
			}
			else
			{
				if (context.keys[i].bPrev)
				{
					context.keys[i].eState = KEY_STATE::KEYUP;
				}
				else
				{
					context.keys[i].eState = KEY_STATE::NONE;
				}
				context.keys[i].bPrev = false;
			}
		}
	}

	else
	{
		for (int i = 00; i < (int)KEY::LAST; ++i)
		{
			context.keys[i].bPrev = false;

			if (context.keys[i].eState == KEY_STATE::KEYDOWN || context.keys[i].eState == KEY_STATE::PRESS)
			{
				context.keys[i].eState = KEY_STATE::KEYUP;
			}
			else if (context.keys[i].eState == KEY_STATE::KEYUP)
			{
				context.keys[i].eState = KEY_STATE::NONE;
			}
		}
	}

	// 마우스 위치는 메인 창 기준 좌표로 둔다
	POINT cursor = {};
	GetCursorPos(&cursor);
	ScreenToClient(hMainHwnd, &cursor);
	Vec2D nextMousePos(cursor);
	context.mouse.delta = nextMousePos - context.mouse.pos;
	context.mouse.pos = nextMousePos;

	// 휠은 창 메시지로 들어온 만큼 모았다가 이번 프레임 값으로 넘긴다
	context.mouse.wheel = wheelInput;
	wheelInput = 0;

	if (context.keys[(int)KEY::TAB].eState == KEY_STATE::KEYDOWN)
	{
		UIMgr::GetInst()->ToggleMap();
	}
	bool isDragging = context.keys[(int)KEY::LBUTTON].eState == KEY_STATE::PRESS && (context.mouse.delta.x != 0.f || context.mouse.delta.y != 0.f);
	if (context.mouse.wheel != 0 || isDragging)
	{
		UIMgr::GetInst()->Execute(context);
	}
}