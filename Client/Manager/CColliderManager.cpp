#include "../pch.h"
#include "CColliderManager.h"
#include "CSceneManager.h"
#include "../Component/CCollider.h"
#include "../Scene/CScene.h"
#include "../Object/CObject.h"

CColliderManager::CColliderManager()	:
	m_arrCheck{}
{
}

CColliderManager::~CColliderManager()
{
}

void CColliderManager::CollisionGroupUpdate(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight)
{
	// ���� Scene�� �����´�.
	CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();

	// ���� �浹ü�� ������ �浹ü�� �׷��� ��ü���� �����´�.
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	// �浹 ������ ���� ���� �ݺ��ڸ� �����Ѵ�.
	map<ULONGLONG, bool>::iterator iter;

	// ���� �浹ü�� ������ �浹ü�� ��ü�� ���� �浹�� �˻��Ѵ�.
	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// �浹ü�� �������� �ʴ� ���
		if (nullptr == vecLeft[i]->GetCollider())
		{
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// �浹ü�� ���ų�, �ڱ� �ڽŰ��� �浹�� ���
			if (nullptr == vecRight[j]->GetCollider() ||
				vecLeft[i] == vecRight[j])
			{
				continue;
			}

			// �˻� ���� ���ʰ� ������ ��ü�� �浹ü�� �����´�.
			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// �� �浹ü ���� ���̵� ����
			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			// �浹 ������ �� ��� ������ ��� ���(�浹���� �ʾҴ� ��)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{
				// ���� �浹 ���̴�.
				if (iter->second)
				{
					// �������� �浹 �ϰ� �־���.
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// �ٵ� ���� �ϳ��� ���� �����̶��, �浹 ���������ش�.
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
					// �������� �浹���� �ʾҴ�.
					// �ٵ� ���� �ϳ��� ���� �����̶��, �浹���� ���������� ���	
					if (!(vecLeft[i]->IsDead()) && !(vecRight[j]->IsDead()))
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}

			else
			{
				// ���� �浹�ϰ� ���� �ʴ�.
				if (iter->second)
				{
					// �������� �浹�ϰ� �־���.
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
	// ���� �浹ü�� ��ġ�� ũ�⸦ �����´�.
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	// ������ �浹ü�� ��ġ�� ũ�⸦ �����´�.
	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	// �� �浹ü ���� �Ÿ��� �� �浹ü�� ũ���� ������ �պ��� ������ �浹�� ����
	if (abs(vRightPos.x - vLeftPos.x) <= (vRightScale.x + vLeftScale.x) / 2.f &&
		abs(vRightPos.y - vLeftPos.y) <= (vRightScale.y + vLeftScale.y) / 2.f)
	{
		return true;
	}

	return false;
}

void CColliderManager::Update()
{
	for (UINT iRow = 0; iRow < (UINT)OBJECT_TYPE::END; ++iRow)
	{
		for (UINT iCol = iRow; iCol < (UINT)OBJECT_TYPE::END; ++iCol)
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
	// �� ���� ���� �׷� Ÿ���� ������,
	// ū ���� ��(��Ʈ)�� ���

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iRow > iCol)
	{
		UINT temp = (UINT)iRow;
		iRow = (UINT)iCol;
		iCol = (UINT)temp;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
		m_arrCheck[iRow] &= ~(1 << iCol);
	else
		// ��Ʈ����
		m_arrCheck[iRow] |= (1 << iCol);
}