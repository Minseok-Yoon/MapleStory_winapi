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

	CObject*		m_pOwner;		// Collider�� �����ϰ� �ִ� ������Ʈ
	Vec2			m_vOffsetPos;	// ������Ʈ�� ���� ������� ��ġ
	Vec2			m_vFinalPos;	// finalUpdate���� �� �����Ӹ��� ���
	Vec2			m_vScale;		// �浹ü�� ũ��
	
	UINT			m_iID;			// �浹ü ������ ID ��
	UINT			m_iCol;
	
	bool			m_bActive;

	int				m_iLayer;

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

	void SetActive(bool _bActive) { m_bActive = _bActive; }

	void SetLayer(int layer) { m_iLayer = layer; }
	int GetLayer() const { return m_iLayer; }

public:
	// �浹 ���� �Լ�
	void OnCollision(CCollider* _ColTag, CCollider* _pOther); // �浹 ���� ��� ȣ��Ǵ� �Լ�
	void OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther);	// �浹 ���� ��
	void OnCollisionExit(CCollider* _ColTag, CCollider* _pOther);	// �浹 ���� ��

	// ���� �����ڸ� ������ �ʰ� �ϱ� ����
	CCollider& operator = (const CCollider& _origin) = delete;

public:
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};