#pragma once
#include <vector>
#include "Vec2D.h"

enum class KEY_STATE
{
	KEYDOWN,
	KEYUP,
	PRESS,
	NONE,
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	LCONTROL,
	LMENU,		//ALT
	LSHIFT,
	ENTER,
	SPACE,
	ESCAPE,
	TAB,
	CAPITAL,	//CAPSLOCK

	LBUTTON,
	RBUTTON,

	LAST,		// 마지막 키
};

struct tKeyInfo 
{
	KEY_STATE	eState;	// 현재의 상태
	bool		bPrev;	// 이전 프레임의 입력 상태
};
class InputMgr 
{
	SINGLE(InputMgr);

private:
	vector<tKeyInfo> m_vecKey;
	Vec2D mousePos;
	Vec2D mouseDelta;
	int wheelInput;
	int mouseWheel;

public:
	void init();
	void update();

	KEY_STATE GetKeyState(KEY eKey) 
	{
		return m_vecKey[(int)eKey].eState;
	}
	Vec2D GetMousePos() { return mousePos; }
	Vec2D GetMouseDelta() { return mouseDelta; }
	void AddMouseWheel(int delta) { wheelInput += delta; }
	int GetMouseWheel() { return mouseWheel; }
};