#include "pch.h"
#include "Object.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

Object::Object()
	: m_Pos{}
	, m_objScale{}
	, m_pCollider(nullptr)
	, m_ObjName()
	, m_bisDead(false)
	, m_pAnimator(nullptr)
{
}

Object::Object(const Object& origin)
	: m_Pos{ origin.m_Pos }
	, m_objScale{ origin.m_objScale }
	, m_pCollider(nullptr)
	, m_ObjName(origin.m_ObjName)
	, m_bisDead(false)
	, m_pAnimator(nullptr)
{
	if (origin.m_pCollider)
	{
		m_pCollider = new Collider(*origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	if (origin.m_pAnimator)
	{
		m_pAnimator = new Animator(*origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}
}

Object::~Object()
{
	if (m_pCollider != nullptr)
	{
		delete m_pCollider;
	}

	if (m_pAnimator != nullptr)
	{
		delete m_pAnimator;
	}
}

void Object::CreateCollider()
{
	m_pCollider = new Collider;
	m_pCollider->m_pOwner = this;
}

void Object::CreateAnimator()
{
	m_pAnimator = new Animator;
	m_pAnimator->m_pOwner = this;
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
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->render(hdc);
	}
}


