#include "pch.h"
#include "MonsterController.h"
#include "Animator.h"
#include "TimeMgr.h"

const BehaviorMask<MonsterState, MonsterBehavior> MonsterController::allowedBehaviors =
{
	{ false, false },	// None
	{ true,  true  },	// Alive
	{ false, false },	// Invincible
	{ false, false },	// Dead
};

bool MonsterController::Idle()
{
	if (!allowedBehaviors.Allows(owner->GetState(), MonsterBehavior::Idle))
	{
		return false;
	}

	owner->GetAnimator()->Play(ObjType::Monster, L"Idle", true, owner->GetCurrDir());

	return true;
}

bool MonsterController::Move(Vec2D direction)
{
	if (!allowedBehaviors.Allows(owner->GetState(), MonsterBehavior::Move))
	{
		return false;
	}

	Vec2D position = owner->GetPos();
	position.x += direction.x * owner->GetSpeed() * fDT;
	position.y += direction.y * owner->GetSpeed() * fDT;

	bool isSide = abs(direction.x) >= abs(direction.y);

	if (isSide)
	{
		owner->SetIsRight(direction.x > 0.f);
	}

	owner->SetCurrDir(isSide ? L"Side" : (direction.y < 0.f ? L"Up" : L"Down"));
	owner->SetPos(position);

	owner->GetAnimator()->Play(ObjType::Monster, L"Walk", true, owner->GetCurrDir());

	return true;
}

MonsterController::MonsterController(Monster* ownerMonster)
	: Controller(ownerMonster)
{
}

MonsterController::~MonsterController()
{
}

void MonsterController::update()
{
	Vec2D direction = owner->IsRight() ? Vec2D(1.f, 0.f) : Vec2D(-1.f, 0.f);
	float offset = owner->GetPos().x - owner->GetCenterPos().x;

	if (direction.x > 0.f && offset >= owner->GetMaxMoveDistance())
	{
		direction = Vec2D(-1.f, 0.f);
	}
	else if (direction.x < 0.f && offset <= -owner->GetMaxMoveDistance())
	{
		direction = Vec2D(1.f, 0.f);
	}

	Move(direction);
}