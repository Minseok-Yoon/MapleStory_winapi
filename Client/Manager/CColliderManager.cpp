#include "../pch.h"
#include "CColliderManager.h"
#include "CSceneManager.h"
#include "../Component/CCollider.h"
#include "../Component/CColliderPixel.h"
#include "../Scene/CScene.h"
#include "../Object/CObject.h"
#include "../Core/CCore.h"

CColliderManager::CColliderManager() :
	m_arrCheck{}
{
}

CColliderManager::~CColliderManager()
{
}

void CColliderManager::CollisionGroupUpdate(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight)
{
    CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();

    const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
    const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

    for (auto pLeftObj : vecLeft)
    {
        const vector<CCollider*>& vecLeftColliders = pLeftObj->GetCollider();
        for (auto pLeftCol : vecLeftColliders)
        {
            for (auto pRightObj : vecRight)
            {
                if (pLeftObj == pRightObj)
                {
                    continue;
                }

                const vector<CCollider*>& vecRightColliders = pRightObj->GetCollider();
                for (auto pRightCol : vecRightColliders)
                {
                    ULONGLONG leftID = pLeftCol->GetID();
                    ULONGLONG rightID = pRightCol->GetID();

                    COLLIDER_ID colID;
                    colID.Left_id = leftID;
                    colID.Right_id = rightID;

                    bool isColliding = IsCollision(pLeftCol, pRightCol);

                    if (isColliding)
                    {
                        if (m_mapColInfo[colID.ID])
                        {
                            if (pLeftObj->IsDead() || pRightObj->IsDead())
                            {
                                pLeftCol->OnCollisionExit(pLeftCol, pRightCol);
                                pRightCol->OnCollisionExit(pRightCol, pLeftCol);
                                m_mapColInfo[colID.ID] = false;
                            }
                            else
                            {
                                pLeftCol->OnCollision(pLeftCol, pRightCol);
                                pRightCol->OnCollision(pRightCol, pLeftCol);
                            }
                        }
                        else
                        {
                            if (!pLeftObj->IsDead() && !pRightObj->IsDead())
                            {
                                pLeftCol->OnCollisionEnter(pLeftCol, pRightCol);
                                pRightCol->OnCollisionEnter(pRightCol, pLeftCol);
                                m_mapColInfo[colID.ID] = true;
                            }
                        }
                    }
                    else
                    {
                        if (m_mapColInfo[colID.ID])
                        {
                            pLeftCol->OnCollisionExit(pLeftCol, pRightCol);
                            pRightCol->OnCollisionExit(pRightCol, pLeftCol);
                            m_mapColInfo[colID.ID] = false;
                        }
                    }
                }
            }
        }
    }
}

bool CColliderManager::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}

	return false;
}

void CColliderManager::Update()
{
	for (UINT iRow = 0; iRow < (UINT)OBJECT_TYPE::END; ++iRow)
	{
		for (UINT iCol = 0; iCol < (UINT)OBJECT_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionGroupUpdate((OBJECT_TYPE)iRow, (OBJECT_TYPE)iCol);
			}
		}
	}
}

void CColliderManager::CheckGroup(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight)
{
	// 더 작은 값의 그룹 타입을 행으로,
	// 큰 값을 열(비트)로 사용

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iRow > iCol)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		// 비트연산
		m_arrCheck[iRow] |= (1 << iCol);
	}
}