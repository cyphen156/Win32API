#include "pch.h"
#include "Player.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"


Player::Player() : m_fspeed(100.f), m_pTex(nullptr)
{
	// 리소스 로딩 - Texture
	m_pTex = ResMgr::GetInst()->LoadTex(L"PlayerTexture", L"texture\\curr.bmp");

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, 0.f));
	GetCollider()->SetColliderSize(Vec2D(100.f, 100.f));
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

void Player::render(HDC hdc)
{
	Vec2D vPlayerPos = GetPos();

	UINT iWidth = m_pTex->Width();
	UINT iHeight = m_pTex->Height();
	
	TransparentBlt(hdc
		, int(vPlayerPos.x - (float)(iWidth / 2))
		, int(vPlayerPos.y - (float)(iWidth / 2))
		, (int)iWidth
		, (int)iHeight
		, m_pTex->GetDC()
		, 0
		, 0
		, (int)iWidth
		, (int)iHeight
		, RGB(255, 0, 255));

	componentRender(hdc);
}