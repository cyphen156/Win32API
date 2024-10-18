#pragma once
#include "Object.h"

class Object;

struct tEvent
{
	EVENT_TYPE	eEvent;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};

class EventMgr
{
	SINGLE(EventMgr);

private:
	vector<tEvent>	m_vecEvent;
	vector<Object*>	m_vecDeadObj;

	void Execute(const tEvent& event);

public:
	void update();
	void AddEvent(const tEvent& event);
};

