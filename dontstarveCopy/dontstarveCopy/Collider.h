#pragma once

#include "Object.h"

class Object;

class Collider
{
private:
	Object* m_pOwner;

	Vec2D	m_vOffSetPos;
	Vec2D	m_vFinalPos;

	Vec2D	m_vColliderSize;

	static UINT	g_iNextID;

	UINT	m_iColliderID;
	bool	m_bCol;

public:
	Collider();
	Collider(const Collider& origin);
	~Collider();

	void SetOffsetPos(Vec2D vPos) { m_vOffSetPos = vPos; }
	void SetColliderSize(Vec2D size) { m_vColliderSize = size; }

	Vec2D GetOffsetPos() { return m_vOffSetPos; }
	Vec2D GetColliderSize() { return m_vColliderSize; }

	UINT GetColliderID() { return m_iColliderID; }
	Vec2D GetFinalPos() { return m_vFinalPos; }
	Object* GetObj() { return m_pOwner; }
	void finalupdate();
	void render(HDC hdc);

	void OnCollision(Collider* pOther);
	void OnCollisionEnter(Collider* pOther);
	void OnCollisionExit(Collider* pOther);
	Collider& operator = (Collider& origin) = delete;

	friend class Object;
};

