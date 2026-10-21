#include "pch.h"
#include "Collider.h"
#include "CameraMgr.h"
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
	//m_vFinalPos = ObjectPos + m_vOffSetPos;

}

void Collider::render(HDC hdc)
{
	SelectGDI p (hdc, PEN_TYPE::RED);
	SelectGDI b (hdc, BRUSH_TYPE::HOLLOW);

	Vec2D colSize = GetColliderSize();
	Vec2D ownerScale = m_pOwner->GetScale();
	Vec2D ownerAnchor = m_pOwner->GetFootAnchor();
	Vec2D ownerFoot = m_pOwner->GetPos() + Vec2D(ownerScale.x / 2.f + ownerAnchor.x, ownerScale.y / 2.f + ownerAnchor.y);
	float zoom = CameraMgr::GetInst()->GetZoom();
	Vec2D renderCenter = CameraMgr::GetInst()->GetRenderPos(ownerFoot) + (m_vFinalPos - ownerFoot) * Vec2D(zoom, zoom);

	Rectangle(hdc
		, (int)(renderCenter.x - colSize.x * zoom / 2)
		, (int)(renderCenter.y - colSize.y * zoom / 2)
		, (int)(renderCenter.x + colSize.x * zoom / 2)
		, (int)(renderCenter.y + colSize.y * zoom / 2));
	colSize = GetColliderSize();

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
