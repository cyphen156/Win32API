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
	
	map<ULONGLONG, bool>	m_mapColInfo;		// ���� �����ӿ����� �浹ü�� �浹����
	UINT	m_arrCheck[(UINT)ObjType::END];	// �׷찣 �浹 üũ ����

	void CollisionObjUpdate(ObjType left, ObjType right);
	bool isCollision(Collider* pLColli,Collider* pRColli);

public:
	void update();
	void CheckObjType(ObjType left, ObjType right);
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)ObjType::END); }
};

