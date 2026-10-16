#include "pch.h"
#include "EventMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

EventMgr::EventMgr() {}

EventMgr::~EventMgr() {}

void EventMgr::Execute(const tEvent& event)
{
	EVENT_TYPE evtType = event.eEvent;

	switch (evtType)
	{
		case EVENT_TYPE::CREATE_OBJ:
		{
			Object* newObj = (Object*)event.lParam;
			ObjType objType = (ObjType)event.wParam;

			SceneMgr::GetInst()->GetCurScene()->AddObj(newObj, objType);
			break;
		}
		case EVENT_TYPE::DELETE_OBJ:
		{
			Object* deadObj = (Object*)event.lParam;
			deadObj->SetDead();
			m_vecDeadObj.push_back(deadObj);
			break;
		}
		case EVENT_TYPE::CHANGE_SCENE:
		{
			SceneMgr::GetInst()->ChangeScene((SceneType)event.lParam);
			break;
		}
	}
}

void EventMgr::update()
{
	for (size_t i = 0; i < m_vecDeadObj.size(); ++i)
	{
		delete m_vecDeadObj[i];
	}
	m_vecDeadObj.clear();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Execute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

void EventMgr::AddEvent(const tEvent& event)
{
	m_vecEvent.push_back(event);
}
