#pragma once
#include "Controller.h"
#include "BehaviorMask.h"
#include "Monster.h"

class MonsterController : public Controller<Monster>
{
private:
	static const BehaviorMask<MonsterState, MonsterBehavior> allowedBehaviors;

	bool Idle();
	bool Move(Vec2D direction);

public:
	MonsterController(Monster* ownerMonster);
	~MonsterController() override;

	void update() override;
};