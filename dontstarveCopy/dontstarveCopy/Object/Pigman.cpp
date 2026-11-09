#include "pch.h"
#include "Pigman.h"
#include "Animator.h"
#include "Animation.h"

Pigman::Pigman()
{
	SetMonsterType(MonsterType::Pigman);
	SetHp(250);
	SetAtk(35);
	SetInvincibleDuration(14.f / 30.f);

	// 리소스 로딩 - Texture
	wstring resPath = L"Pigman";
	GenerateAnimations(resPath);
	Texture* pTex = GetAnimator()->FindAnimation(L"Pigman_Idle_Down")->GetFrame(0);
	Image* img = pTex->GetImage();
	float width = (float)img->GetWidth();
	float height = (float)img->GetHeight();
	Vec2D baseScale(width, height);
	SetScale(baseScale);

	// 충돌체는 대기 첫 프레임에서 그림이 차지하는 영역
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, -31.5f));
	GetCollider()->SetColliderSize(Vec2D(52.f, 65.f));
}

Pigman::~Pigman()
{
}