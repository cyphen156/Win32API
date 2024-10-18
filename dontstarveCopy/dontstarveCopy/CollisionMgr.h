#pragma once
#include "Collider.h"
#include <map>

class Collider;

union ColliderID
{
	struct
	{
		UINT	iLeftId;
		UINT	iRightID;
	}; LONGLONG ID;
};

class CollisionMgr
{
	SINGLE(CollisionMgr);

private:
	
	map<ULONGLONG, bool>	m_mapColInfo;		// 이전 프레임에서의 충돌체간 충돌정보
	UINT	m_arrCheck[(UINT)ObjType::END];	// 그룹간 충돌 체크 집합

	void CollisionObjUpdate(ObjType left, ObjType right);
	bool isCollision(Collider* pLColli,Collider* pRColli);

public:
	void update();
	void CheckObjType(ObjType left, ObjType right);
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)ObjType::END); }
};

