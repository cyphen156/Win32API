#include "pch.h"
#include "Monster.h"
#include "TimeMgr.h"

Monster::Monster() : 
	m_vCenterPos(Vec2D(0.f, 0.f))
	, m_fspeed(100.f)
	, m_fMaxDist(50.f)
	, m_iDirection(1)
{
}

Monster::~Monster()
{
}

void Monster::update()
{
	Vec2D vPos = GetPos();

	vPos.x += m_fspeed * m_iDirection * fDT;

	float fDist = abs(m_vCenterPos.x - vPos.x) - m_fMaxDist;

	if (fDist > 0)
	{
		m_iDirection *= -1;

		vPos.x += fDist * m_iDirection;
	}

	SetPos(vPos);
}

void Monster::render()
{
}
