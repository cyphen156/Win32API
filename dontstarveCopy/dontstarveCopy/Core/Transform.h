#pragma once
#include "Vec2D.h"

struct Transform
{
	Vec2D position;
	Vec2D scale;
	Transform* parent;

	Transform() : position{}, scale(1.f, 1.f), parent(nullptr) {}
	Transform(const Vec2D& position, const Vec2D& scale, Transform* parent) : position(position), scale(scale), parent(parent) {}

	XFORM GetCombinedMatrix()
	{
		XFORM local = { scale.x, 0.f, 0.f, scale.y, position.x, position.y };
		if (parent == nullptr)
		{
			return local;
		}
		XFORM parentFinal = parent->GetCombinedMatrix();
		XFORM combined = {};
		CombineTransform(&combined, &local, &parentFinal);
		return combined;
	}

	Transform GetFinalTransform()
	{
		if (parent == nullptr)
		{
			return *this;
		}
		Transform parentFinal = parent->GetFinalTransform();
		return Transform(parentFinal.position + parentFinal.scale * position, parentFinal.scale * scale, nullptr);
	}
};