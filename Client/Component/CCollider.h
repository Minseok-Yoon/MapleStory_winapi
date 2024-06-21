#pragma once

class CObject;

class CCollider
{
	friend class CObject;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

private:
	static UINT		g_iNextID;

	CObject*		m_pOwner;		// Collidr를 소유하고 있는 오브젝트
	Vec2			m_vOffsetPos;	// 오브젝트로 부터 상대적인 위치
	Vec2			m_vFinalPos;	// finalUpdate에서 매 프레임마다 계산
	Vec2			m_vScale;		// 충돌체의 크기
	
	UINT			m_iID;			// 충돌체 고유한 ID 값
	UINT			m_iCol;
	
	bool			m_bActive;


public:
	string			m_strColTag;

public:
	void SetOwner(CObject* _pOwner) { m_pOwner = _pOwner; }
	CObject* GetOwner() const { return m_pOwner; }

	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	Vec2 GetOffsetPos() { return m_vOffsetPos; }

	void SetFinalPos(Vec2 _vPos) { m_vFinalPos = _vPos; }
	Vec2 GetFinalPos() { return m_vFinalPos; }

	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	void SetColTag(string _strColTag) { m_strColTag = _strColTag; }
	string GetColTag() { return m_strColTag; }

	UINT GetID() { return m_iID; }

public:
	// 충돌 시점 함수
	void OnCollision(CCollider* _pOther); // 충돌 중인 경우 호출되는 함수
	void OnCollisionEnter(CCollider* _pOther);	// 충돌 진입 시
	void OnCollisionExit(CCollider* _pOther);	// 충돌 해제 시

	// 대입 연산자를 만들지 않게 하기 위해
	CCollider& operator = (const CCollider& _origin) = delete;

public:
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};