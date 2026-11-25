#include "pch.h"
#include "InputMgr.h"
#include "Core.h"
#include "UIMgr.h"
#include "EventMgr.h"
#include <windowsx.h>

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
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		for (int i = 00; i < (int)KEY::LAST; ++i)
		{
			switch (context.keys[i].eState)
			{
			case KEY_STATE::KEYDOWN:
				context.keys[i].eState = KEY_STATE::PRESS;
				break;
			case KEY_STATE::KEYUP:
				context.keys[i].eState = KEY_STATE::NONE;
				break;
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

	const vector<tEvent>& events = EventMgr::GetInst()->GetEvents();
	for (size_t i = 0; i < events.size(); ++i)
	{
		switch (events[i].eEvent)
		{
		case EVENT_TYPE::MOUSE_MOVE:
			context.mouse.pos = Vec2D(GET_X_LPARAM(events[i].lParam), GET_Y_LPARAM(events[i].lParam));
			break;
		case EVENT_TYPE::KEY_INPUT:
			for (int j = 0; j < (int)KEY::LAST; ++j)
			{
				if (g_arrVK[j] != (int)events[i].lParam)
				{
					continue;
				}
				if (context.keys[j].bPrev == ((KEY_STATE)events[i].wParam == KEY_STATE::KEYDOWN))
				{
					continue;
				}
				context.keys[j].eState = (KEY_STATE)events[i].wParam;
				context.keys[j].bPrev = context.keys[j].eState == KEY_STATE::KEYDOWN;

				switch ((KEY)j)
				{
				case KEY::TAB:
					if (context.keys[j].eState == KEY_STATE::KEYDOWN)
					{
						UIMgr::GetInst()->ToggleMap();
					}
					break;
				case KEY::LBUTTON:
				case KEY::RBUTTON:
					context.mouse.delta = context.mouse.pos - previousMousePos;
					previousMousePos = context.mouse.pos;
					context.mouse.wheel = 0;
					UIMgr::GetInst()->Execute(context);
					context.keys[j].eState = context.keys[j].bPrev ? KEY_STATE::PRESS : KEY_STATE::NONE;
					break;
				}
			}
			break;
		}
	}

	context.mouse.delta = context.mouse.pos - previousMousePos;
	previousMousePos = context.mouse.pos;

	// 휠은 창 메시지로 들어온 만큼 모았다가 이번 프레임 값으로 넘긴다
	context.mouse.wheel = wheelInput;
	wheelInput = 0;

	if (context.mouse.wheel != 0 || context.keys[(int)KEY::LBUTTON].eState != KEY_STATE::NONE || context.keys[(int)KEY::RBUTTON].eState != KEY_STATE::NONE)
	{
		UIMgr::GetInst()->Execute(context);
	}
}