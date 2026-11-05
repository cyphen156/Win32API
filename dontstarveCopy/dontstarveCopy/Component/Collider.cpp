#include "pch.h"
#include "Collider.h"
#include "Core.h"
#include "SelectGDI.h"

UINT Collider::g_iNextID = 0;

Collider::Collider()
	: m_pOwner(nullptr)
	, m_iColliderID(g_iNextID++)
	, m_bCol(false)
{
}

Collider::Collider(const Collider& origin)
	: m_pOwner(nullptr)
	, m_vOffSetPos(origin.m_vOffSetPos)
	, m_vColliderSize(origin.m_vColliderSize)
	, m_iColliderID(g_iNextID++)
	, m_bCol(false)
{
	UINT m_iColliderId;
}

Collider::~Collider()
{
}

void Collider::finalupdate()
{
	Vec2D ObjectPos = m_pOwner->GetPos();
	m_vFinalPos = ObjectPos + m_vOffSetPos;
}

void Collider::render(HDC hdc)
{
#ifdef _DEBUG
	Vec2D colSize = GetColliderSize();

	XFORM transform;
	GetWorldTransform(hdc, &transform);
	ModifyWorldTransform(hdc, nullptr, MWT_IDENTITY);
	{
		Graphics graphics(hdc);
		Matrix matrix(transform.eM11, transform.eM12, transform.eM21, transform.eM22, transform.eDx, transform.eDy);
		graphics.SetTransform(&matrix);

		Pen pen(Color(255, 255, 0, 0));
		graphics.DrawRectangle(&pen, m_vFinalPos.x - colSize.x / 2.f, m_vFinalPos.y - colSize.y / 2.f, colSize.x, colSize.y);
	}
	SetWorldTransform(hdc, &transform);
#endif
}

void Collider::OnCollision(Collider* pOther)
{
	m_pOwner->OnCollision(pOther);
}

void Collider::OnCollisionEnter(Collider* pOther)
{
	m_pOwner->OnCollisionEnter(pOther);
	m_bCol = true;
}

void Collider::OnCollisionExit(Collider* pOther)
{
	m_pOwner->OnCollisionExit(pOther);
	m_bCol = false;
}
