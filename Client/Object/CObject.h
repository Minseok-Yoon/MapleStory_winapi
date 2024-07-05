#pragma once
#include "../Manager/CCamera.h"
#include "../Component/CCollider.h"

class CCollider;
class CColliderPixel;
class CAnimator;
class CGravity;

// �浹 ó��, �߷�, �ִϸ��̼� �� �پ��� ����� ���� ���� ������Ʈ Ŭ����
class CObject
{
	friend class CEventManager;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

public:
	wstring			m_strObjName;		// Object�� �̸�
	Vec2			m_vPos;				// Object�� ��ġ
	Vec2			m_vScale;			// Object�� ũ��
	Vec2			m_vDir;				// Object�� ����

	bool			m_bAlive;

	// Component
	vector<CCollider*>	m_vecCollider;
	CColliderPixel*		m_pPixelCollider;
	CAnimator*			m_pAnimator;
	CGravity*			m_pGravity;


public:
	// ������Ʈ�� �̸� ���� �� ��ȯ
	void SetObjName(const wstring& _strObjName) { m_strObjName = _strObjName; }
	const wstring& GetObjName() { return m_strObjName; }

	// ������Ʈ�� ��ġ ���� �� ��ȯ
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }

	// ������Ʈ�� ũ�� ���� �� ��ȯ
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	// ������Ʈ�� ���� ���� �� ��ȯ
	void SetDir(Vec2 _vDir) { m_vDir = _vDir; _vDir.Normalize(); }
	Vec2 GetDir() { return m_vDir; }

	// ������Ʈ�� ���� ���� ��ȯ
	bool IsDead() { return !m_bAlive; }

	void SetPixelCollider(CColliderPixel* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

public:
	const vector<CCollider*>& GetCollider() const { return m_vecCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CGravity* GetGravity() { return m_pGravity; }

private:
	void SetDead() { m_bAlive = false; }

public:
	void AddCollider();
	void CreateAnimator();
	void CreateGravity();

	virtual void OnCollision(CCollider* _ColTag, CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _ColTag, CCollider* _pOther) {}

public:
	virtual void FastUpdate() {}; 	// Scene�� ���۵Ǳ� ������ ȣ��Ǵ� �Լ�
	virtual void Update() = 0;
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);

	void ComponentRender(HDC _dc);

	virtual CObject* Clone() = 0;
};