#include "pch.h"
#include "Monster.h"
#include "TimeMgr.h"

Monster::Monster()
	: m_vCenterPos(Vec2D(0.f, 0.f))
	, m_fspeed(100.f)
	, m_fMaxDist(50.f)
	, m_iDirection(1)
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, 0.f));
	GetCollider()->SetColliderSize(Vec2D(100.f, 100.f));
}

Monster::Monster(const Monster& origin)
	: Object(origin)
	, m_vCenterPos(origin.m_vCenterPos)
	, m_fspeed(origin.m_fspeed)
	, m_fMaxDist(origin.m_fMaxDist)
	, m_iDirection(origin.m_iDirection)
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, 0.f));
	GetCollider()->SetColliderSize(Vec2D(100.f, 100.f));
}
Monster::~Monster()
{
}

void Monster::OnCollisionEnter(Collider* pOther)
{
	Object* pOtherObj = pOther->GetObj();
	if (pOtherObj->GetObjName() == L"Player")
	{
		DeleteObject(this);
	}
}

void Monster::GetAnimations(wstring& TexPath)
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

void Monster::render(HDC hdc)
{
}
