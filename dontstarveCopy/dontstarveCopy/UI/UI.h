#pragma once
#include "Transform.h"

struct InputContext;

class UI
{
protected:
	wstring name;
	Transform transform;
	bool isOpen;

	virtual void OnOpen() {}
	virtual void OnClose() {}

public:
	UI(const wstring& name);
	virtual ~UI();

	virtual void update() = 0;
	virtual void render(HDC hdc) = 0;

	virtual void Execute(const InputContext& ctx) {}
	virtual void Refresh() {}
	virtual void Resize() {}

	const wstring& GetName() { return name; }

	bool IsOpen() { return isOpen; }
	void SetOpen(bool open);

	void SetParent(Transform* parent);
};