#include "pch.h"
#include "PlayerController.h"
#include "Animator.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "CameraMgr.h"
#include "EventMgr.h"
#include "CollisionMgr.h"

const BehaviorMask<PlayerState, PlayerBehavior> PlayerController::allowedBehaviors =
{
	{ false, false, false },	// None
	{ true,  true,  true  },	// Alive
	{ false, false, false },	// Invincible
	{ false, false, false },	// Dead
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

bool PlayerController::Punch()
{
	if (!allowedBehaviors.Allows(owner->GetState(), PlayerBehavior::Punch))
	{
		return false;
	}

	Vec2D facing(0.f, 1.f);
	if (owner->GetCurrDir() == L"Up")
	{
		facing = Vec2D(0.f, -1.f);
	}
	if (owner->GetCurrDir() == L"Side")
	{
		facing = Vec2D(owner->IsRight() ? 1.f : -1.f, 0.f);
	}

	Vec2D center(owner->GetPos().x + facing.x * owner->GetAtkRange() / 2.f, owner->GetPos().y + facing.y * owner->GetAtkRange() / 2.f);
	Vec2D size = facing.x != 0.f ? Vec2D(owner->GetAtkRange(), owner->GetAtkWidth()) : Vec2D(owner->GetAtkWidth(), owner->GetAtkRange());

	Collider* hit = CollisionMgr::GetInst()->OverlapBox(center, size, ObjType::Monster);
	if (hit == nullptr)
	{
		return false;
	}

	IHittable* target = dynamic_cast<IHittable*>(hit->GetOwner());
	if (target == nullptr)
	{
		return false;
	}

	owner->GetAnimator()->Play(ObjType::Player, L"Punch", false, owner->GetCurrDir());
	isPunching = true;

	tEvent event = {};
	event.eEvent = EVENT_TYPE::HIT;
	event.lParam = reinterpret_cast<DWORD_PTR>(target);
	event.wParam = static_cast<DWORD_PTR>(owner->GetAtk());

	EventMgr::GetInst()->AddEvent(event);

	return true;
}

PlayerController::PlayerController(Player* ownerPlayer)
	: Controller(ownerPlayer)
	, isPunching(false)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::update()
{
	int wheel = InputMgr::GetInst()->GetMouseWheel();
	if (wheel > 0)
	{
		CameraMgr::GetInst()->ZoomIn();
	}
	else if (wheel < 0)
	{
		CameraMgr::GetInst()->ZoomOut();
	}

	if (isPunching && !owner->GetAnimator()->IsFinish())
	{
		return;
	}
	isPunching = false;

	if (KEY_DOWN(KEY::SPACE) && Punch())
	{
		return;
	}

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

	if (direction.x != 0.f || direction.y != 0.f)
	{
		Move(direction);
	}
	else
	{
		Idle();
	}
}