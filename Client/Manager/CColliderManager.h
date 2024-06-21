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
	map<ULONGLONG, bool>		m_mapColInfo;						// 충돌체 간의 이전 프레임 충돌 정보
	UINT						m_arrCheck[(UINT)OBJECT_TYPE::END];	// 그룹간의 충돌 체크 매트릭스

private:
	void CollisionGroupUpdate(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);

public:
	void Update();
	void CheckGroup(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight);
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJECT_TYPE::END); }
};