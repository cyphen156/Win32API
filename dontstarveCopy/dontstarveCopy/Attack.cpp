#include "pch.h"
#include "Attack.h"

Attack::Attack() : m_fDir(1.f)
{
}

Attack::~Attack()
{
}

void Attack::SetDir(bool dir)
{
	if (dir)
	{
		m_fDir = -1.f;
	}
	else
	{
		m_fDir = 1.f;
	}
}

void Attack::update()
{
	Vec2D vPos = GetPos();

	SetPos(vPos);
}

void Attack::render(HDC hdc)
{
}
