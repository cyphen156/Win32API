#include "pch.h"
#include "UI.h"

UI::UI(const wstring& name)
	: name(name)
	, isOpen(false)
{
	assert(!name.empty());
}

UI::~UI()
{
}

void UI::SetOpen(bool open)
{
	if (isOpen == open)
	{
		return;
	}
	isOpen = open;
	if (isOpen)
	{
		OnOpen();
	}
	else
	{
		OnClose();
	}
}

void UI::SetParent(Transform* parent)
{
	transform.parent = parent;
}