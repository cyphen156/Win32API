#pragma once
#include "Controller.h"
#include "BehaviorMask.h"
#include "Player.h"

class PlayerController : public Controller<Player>
{
private:
	static const BehaviorMask<PlayerState, PlayerBehavior> allowedBehaviors;

	bool Idle();
	bool Move(Vec2D direction);

public:
	PlayerController(Player* ownerPlayer);
	~PlayerController() override;

	void update() override;
};