#include "pch.h"
#include "Object.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

Object::Object() : m_Pos{}, m_objScale{}, m_pCollider(nullptr), m_bisDead(true)
{
}

Object::~Object()
{
	if (m_pCollider != nullptr)
	{
		delete m_pCollider;
	}
}

void Object::CreateCollider()
{
	m_pCollider = new Collider;
	m_pCollider->m_pOwner = this;
}

void Object::update()
{
}

void Object::finalupdate()
{
	if (m_pCollider)
	{
		m_pCollider->finalupdate();
	}
}

void Object::render(HDC hdc)
{

	componentRender(hdc);
}

void Object::componentRender(HDC hdc)
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->render(hdc);
	}
}


