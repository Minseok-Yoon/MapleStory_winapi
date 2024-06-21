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
	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// 충돌체를 보유하지 않는 경우
		if (nullptr == vecLeft[i]->GetCollider())
		{
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 충돌체가 없거나, 자기 자신과의 충돌인 경우
			if (nullptr == vecRight[j]->GetCollider() ||
				vecLeft[i] == vecRight[j])
			{
				continue;
			}

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			// 충돌 정보가 미 등록 상태인 경우 등록(충돌하지 않았다 로)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌 중이다.
				if (iter->second)
				{
					// 이전에도 충돌 하고 있었다.
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 근데 둘중 하나가 삭제 예정이라면, 충돌 해제시켜준다.
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}

				else
				{
					// 이전에는 충돌하지 않았다.
					// 근데 둘중 하나가 삭제 예정이라면, 충돌하지 않은것으로 취급	
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}

			else
			{
				// 현재 충돌하고 있지 않다.
				if (iter->second)
				{
					// 이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
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