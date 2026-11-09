#pragma once

class IHittable
{
public:
	virtual ~IHittable()
	{
	}

	virtual void OnHit(int damage) = 0;
};