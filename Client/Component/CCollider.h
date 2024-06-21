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

	CObject*		m_pOwner;		// Collidr�� �����ϰ� �ִ� ������Ʈ
	Vec2			m_vOffsetPos;	// ������Ʈ�� ���� ������� ��ġ
	Vec2			m_vFinalPos;	// finalUpdate���� �� �����Ӹ��� ���
	Vec2			m_vScale;		// �浹ü�� ũ��
	
	UINT			m_iID;			// �浹ü ������ ID ��
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
	// �浹 ���� �Լ�
	void OnCollision(CCollider* _pOther); // �浹 ���� ��� ȣ��Ǵ� �Լ�
	void OnCollisionEnter(CCollider* _pOther);	// �浹 ���� ��
	void OnCollisionExit(CCollider* _pOther);	// �浹 ���� ��

	// ���� �����ڸ� ������ �ʰ� �ϱ� ����
	CCollider& operator = (const CCollider& _origin) = delete;

public:
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};