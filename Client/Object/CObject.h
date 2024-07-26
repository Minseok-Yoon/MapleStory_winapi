#pragma once
#include "../Manager/CCamera.h"
#include "../Component/CCollider.h"

class CCollider;
class CColliderPixel;
class CAnimator;
class CGravity;
class CRigidBody;

class CObject
{
	friend class CEventManager;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

protected:
	wstring			m_strObjName;		// Object의 이름
	Vec2			m_vPos;				// Object의 위치
	Vec2			m_vScale;			// Object의 크기
	Vec2			m_vDir;				// Object의 방향
	bool			m_bAlive;

public:
	// Component
	vector<CCollider*>	m_vecCollider;
	CColliderPixel*		m_pPixelCollider;
	CAnimator*			m_pAnimator;
	CGravity*			m_pGravity;
	CRigidBody*			m_pRigidBody;


public:
	// 오브젝트의 이름 설정 및 반환
	void SetObjName(const wstring& _strObjName) { m_strObjName = _strObjName; }
	const wstring& GetObjName() const { return m_strObjName; }

	// 오브젝트의 위치 설정 및 반환
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() const { return m_vPos; }

	// 오브젝트의 크기 설정 및 반환
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() const { return m_vScale; }

	// 오브젝트의 방향 설정 및 반환
	void SetDir(Vec2 _vDir) { m_vDir = _vDir; _vDir.Normalize(); }
	Vec2 GetDir() const { return m_vDir; }

	// 오브젝트의 생존 여부 반환
	bool IsDead() const { return !m_bAlive; }

	void SetPixelCollider(CColliderPixel* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

private:
	void SetDead() { m_bAlive = false; }

public:
	const vector<CCollider*>& GetCollider() const { return m_vecCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CGravity* GetGravity() { return m_pGravity; }
	CRigidBody* GetRigidBody() { return m_pRigidBody; }

public:
	void AddCollider();
	void CreateAnimator();
	void CreateGravity();
	void CreateRigidBody();

	virtual void OnCollision(CCollider* _ColTag, CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _ColTag, CCollider* _pOther) {}

public:
	virtual void FastUpdate() {};
	virtual void Update() = 0;
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
	void ComponentRender(HDC _dc);

	virtual CObject* Clone() = 0;
};