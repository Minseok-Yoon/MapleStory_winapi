#pragma once

class CCollider;

union COLLIDER_ID
{
	struct {
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CColliderManager
{
	SINGLETON(CColliderManager);

private:
	map<ULONGLONG, bool>		m_mapColInfo;						// �浹ü ���� ���� ������ �浹 ����
	UINT						m_arrCheck[(UINT)OBJECT_TYPE::END];	// �׷찣�� �浹 üũ ��Ʈ����

private:
	void CollisionGroupUpdate(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);

public:
	void Update();
	void CheckGroup(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight);
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJECT_TYPE::END); }
};