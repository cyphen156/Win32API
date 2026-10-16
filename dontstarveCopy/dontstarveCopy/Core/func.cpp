#include "pch.h"
#include "func.h"
#include "EventMgr.h"

void CreateObj(Object* createObj, ObjType objType)
{
	tEvent event = {};
	event.eEvent = EVENT_TYPE::CREATE_OBJ;
	event.lParam = (DWORD_PTR)createObj;
	event.wParam = (DWORD_PTR)objType;

	EventMgr::GetInst()->AddEvent(event);
}

void DeleteObj(Object* deleteObj)
{
	tEvent event = {};
	event.eEvent = EVENT_TYPE::DELETE_OBJ;
	event.lParam = (DWORD_PTR)deleteObj;

	EventMgr::GetInst()->AddEvent(event);
}

void ChangeScene(SceneType next)
{
	tEvent event = {};
	event.eEvent = EVENT_TYPE::CHANGE_SCENE;
	event.lParam = (DWORD)next;

	EventMgr::GetInst()->AddEvent(event);
}

