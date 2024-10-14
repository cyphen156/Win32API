#pragma once
#include <vector>

enum class KEY_STATE
{
	KEYDOWN,
	KEYUP,
	PRESS,
	NONE,
};

enum class KEY
{
	VKLEFT,
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
	RETURN,
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
class KeyMgr 
{
	SINGLE(KeyMgr);

private:
	vector<tKeyInfo> m_vecKey;
public:
	void init();
	void update();

	KEY_STATE GetKeyState(KEY eKey) 
	{
		return m_vecKey[(int)eKey].eState;
	}
};