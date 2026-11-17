#pragma once
#include "Transform.h"

class Texture;

class Sprite
{
private:
	Texture* texture;
	Transform transform;

public:
	Sprite();

	void SetTexture(Texture* source) { texture = source; }
	Texture* GetTexture() { return texture; }
	Transform& GetTransform() { return transform; }
};