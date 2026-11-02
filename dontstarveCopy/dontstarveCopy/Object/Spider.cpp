#include "pch.h"
#include "Spider.h"
#include "Animator.h"
#include "Animation.h"

Spider::Spider()
{
	SetMonsterType(MonsterType::Spider);

	// 리소스 로딩 - Texture
	wstring resPath = L"Spider";
	GenerateAnimations(resPath);
	wstring dir = GetCurrDir();
	GetAnimator()->Play(ObjType::Monster, L"Idle", true, dir);
	Texture* pTex = GetAnimator()->FindAnimation(L"Spider_Idle_Down")->GetFrame(0);
	Image* img = pTex->GetImage();
	float width = (float)img->GetWidth();
	float height = (float)img->GetHeight();
	Vec2D baseScale(width, height);
	SetScale(baseScale);

	// 충돌체는 대기 첫 프레임에서 그림이 차지하는 영역
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, -16.f));
	GetCollider()->SetColliderSize(Vec2D(58.f, 30.f));
}

Spider::~Spider()
{
}