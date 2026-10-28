#include "pch.h"
#include "UI.h"

UI::UI()
	: isOpen(false)
{
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
