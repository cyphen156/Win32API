#include "pch.h"
#include "PlayerController.h"
#include "Animator.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "CameraMgr.h"

const BehaviorMask<PlayerState, PlayerBehavior> PlayerController::allowedBehaviors =
{
	{ false, false },	// None
	{ true,  true  },	// Alive
	{ false, false },	// Dead
};

bool PlayerController::Idle()
{
	if (!allowedBehaviors.Allows(owner->GetState(), PlayerBehavior::Idle))
	{
		return false;
	}

	owner->GetAnimator()->Play(ObjType::Player, L"Idle", true, owner->GetCurrDir());

	return true;
}

bool PlayerController::Move(Vec2D direction)
{
	if (!allowedBehaviors.Allows(owner->GetState(), PlayerBehavior::Move))
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

	owner->GetAnimator()->Play(ObjType::Player, L"Run", true, owner->GetCurrDir());

	return true;
}

PlayerController::PlayerController(Player* ownerPlayer)
	: Controller(ownerPlayer)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::update()
{
	Vec2D direction;

	if (KEY_PRESS(KEY::W))
	{
		direction.y -= 1.f;
	}
	if (KEY_PRESS(KEY::A))
	{
		direction.x -= 1.f;
	}
	if (KEY_PRESS(KEY::S))
	{
		direction.y += 1.f;
	}
	if (KEY_PRESS(KEY::D))
	{
		direction.x += 1.f;
	}
	if (KEY_PRESS(KEY::SPACE))
	{
		//attack();, chop
	}

	if (direction.x != 0.f || direction.y != 0.f)
	{
		Move(direction);
	}
	else
	{
		Idle();
	}

	int wheel = InputMgr::GetInst()->GetMouseWheel();
	if (wheel > 0)
	{
		CameraMgr::GetInst()->ZoomIn();
	}
	else if (wheel < 0)
	{
		CameraMgr::GetInst()->ZoomOut();
	}
}