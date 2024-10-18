#include "pch.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

CollisionMgr::CollisionMgr()
	: m_arrCheck{}
{

};

CollisionMgr::~CollisionMgr()
{

};

void CollisionMgr::CollisionObjUpdate(ObjType left, ObjType right)
{
	Scene* pCurScene = SceneMgr::GetInst()->GetCurScene();

	const vector<Object*>& rowObj = pCurScene->GetObjectGroup(left);
	const vector<Object*>& colObj = pCurScene->GetObjectGroup(right);
	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < rowObj.size(); ++i)
	{
		// �浹ü�� ���� ���
		if (rowObj[i]->GetCollider() == nullptr)
		{
			continue;
		}
		for (size_t j = 0; j < colObj.size(); ++j)
		{
			// �浹ü�� ���ų� �ڱ��ڽ��ΰ��
			if (colObj[i]->GetCollider() == nullptr || rowObj[i] == colObj[j])
			{
				continue;
			}
			Collider* pLeft = rowObj[i]->GetCollider();
			Collider* pRight = colObj[j]->GetCollider();

			ColliderID ID;
			ID.iLeftId = pLeft->GetColliderID();
			ID.iRightID = pRight->GetColliderID();

			iter = m_mapColInfo.find(ID.ID);

			// �浹 ���� �̵���� ���
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}
			
			// ���� �浹��
			if (isCollision(pLeft, pRight))
			{
				// �������� �浹��
				if (iter->second) 
				{
					if (rowObj[i]->isDead() || colObj[j]->isDead())
					{
						pLeft->OnCollisionExit(pRight);
						pRight->OnCollisionExit(pLeft);
						iter->second = false;
					}
					else 
					{
						pLeft->OnCollision(pRight);
						pRight->OnCollision(pLeft);
					}
				}
				else
				{
					// �������� �浹 ����
					if (!rowObj[i]->isDead() || !colObj[j]->isDead())
					{
						pLeft->OnCollisionEnter(pRight);
						pRight->OnCollisionEnter(pLeft);
						iter->second = true;
					}
				}
			}
			//���� �浹����
			else
			{
				// �������� �浹
				if (iter->second)
				{
					// �������� �浹 ����
					pLeft->OnCollisionExit(pRight);
					pRight->OnCollisionExit(pLeft);
					iter->second = false;
				}
			}
		}
	}
}

bool CollisionMgr::isCollision(Collider* pLColli, Collider* pRColli)
{
	Vec2D vLeftPos = pLColli->GetFinalPos();
	Vec2D vRightPos = pRColli->GetFinalPos();

	Vec2D vLeftScale = pLColli->GetColliderSize();
	Vec2D vRightScale = pRColli->GetColliderSize();

	if (abs(vLeftPos.x - vRightPos.x) < (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vLeftPos.x - vRightPos.x) < (vLeftScale.x + vRightScale.x) / 2.f)
	{
		return true;
	}
	return false;
}

void CollisionMgr::update()
{
	for (UINT iRow = 0; iRow < (UINT)ObjType::END; ++iRow)
	{
		for (UINT iCol= 0; iCol < (UINT)ObjType::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionObjUpdate((ObjType)iRow, (ObjType)iCol);
			}
		}
	}

}

void CollisionMgr::CheckObjType(ObjType left, ObjType right)
{
	UINT iRow = (UINT)left;
	UINT iCol = (UINT)right;
	if (iRow > iCol)
	{
		iRow = iCol;
		iCol = (UINT)left;
	}
	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}
}
