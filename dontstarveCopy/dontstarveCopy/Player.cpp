#include "pch.h"
#include "Player.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

Player::Player() : m_fspeed(100.f)
{

}

Player::~Player()
{
}

void Player::update()
{
	Vec2D vPos = GetPos();

	if (KEY_PRESS(KEY::W))
	{
		vPos.y -= m_fspeed * fDT;
	}
	if (KEY_PRESS(KEY::A))
	{
		vPos.x -= m_fspeed * fDT;
	}
	if (KEY_PRESS(KEY::S))
	{
		vPos.y += m_fspeed * fDT;
	}
	if (KEY_PRESS(KEY::D))
	{
		vPos.x += m_fspeed * fDT;
	}
	if (KEY_PRESS(KEY::SPACE))
	{
		//attack();
	}

	SetPos(vPos);
}

void Player::render()
{
}
