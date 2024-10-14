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

	LAST,		// ������ Ű
};

struct tKeyInfo 
{
	KEY_STATE	eState;	// ������ ����
	bool		bPrev;	// ���� �������� �Է� ����
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