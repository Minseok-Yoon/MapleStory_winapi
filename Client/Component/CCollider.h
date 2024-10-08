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

	static bool		g_bRenderColliders;  // 충돌체를 렌더링할지 여부를 나타내는 변수

	CObject*		m_pOwner;		// Collider를 소유하고 있는 오브젝트
	Vec2			m_vOffsetPos;	// 오브젝트로 부터 상대적인 위치
	Vec2			m_vFinalPos;	// finalUpdate에서 매 프레임마다 계산
	Vec2			m_vScale;		// 충돌체의 크기
	Vec2			m_vPos;
	
	UINT			m_iID;			// 충돌체 고유한 ID 값
	UINT			m_iCol;
	
	bool			m_bEnable;

	int				m_iLayer;

public:
	string			m_strColTag;

public:
	static void ToggleRenderColliders() { g_bRenderColliders = !g_bRenderColliders; }
	static bool IsRenderCollidersEnabled() { return g_bRenderColliders; }

	void SetOwner(CObject* _pOwner) { m_pOwner = _pOwner; }
	CObject* GetOwner() const { return m_pOwner; }

	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	Vec2 GetOffsetPos() { return m_vOffsetPos; }

	void SetFinalPos(Vec2 _vPos) { m_vFinalPos = _vPos; }
	Vec2 GetFinalPos() { return m_vFinalPos; }

	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }

	void SetColTag(string _strColTag) { m_strColTag = _strColTag; }
	string GetColTag() { return m_strColTag; }

	UINT GetID() { return m_iID; }

	void Enable(bool _bEnable) { m_bEnable = _bEnable; }
	bool IsEnable() const { return m_bEnable; }

	void SetLayer(int layer) { m_iLayer = layer; }
	int GetLayer() const { return m_iLayer; }

public:
	// 충돌 시점 함수
	void OnCollision(CCollider* _ColTag, CCollider* _pOther); // 충돌 중인 경우 호출되는 함수
	void OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther);	// 충돌 진입 시
	void OnCollisionExit(CCollider* _ColTag, CCollider* _pOther);	// 충돌 해제 시

	// 대입 연산자를 만들지 않게 하기 위해
	CCollider& operator = (const CCollider& _origin) = delete;

public:
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};