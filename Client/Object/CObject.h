#pragma once
#include "../Manager/CCamera.h"
#include "../Component/CCollider.h"

class CCollider;
class CColliderPixel;
class CAnimator;
class CGravity;

// 충돌 처리, 중력, 애니메이션 등 다양한 기능을 가진 게임 오브젝트 클래스
class CObject
{
	friend class CEventManager;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

public:
	wstring			m_strObjName;		// Object의 이름
	Vec2			m_vPos;				// Object의 위치
	Vec2			m_vScale;			// Object의 크기
	Vec2			m_vDir;				// Object의 방향

	bool			m_bAlive;

	// Component
	vector<CCollider*>	m_vecCollider;
	CColliderPixel*		m_pPixelCollider;
	CAnimator*			m_pAnimator;
	CGravity*			m_pGravity;


public:
	// 오브젝트의 이름 설정 및 반환
	void SetObjName(const wstring& _strObjName) { m_strObjName = _strObjName; }
	const wstring& GetObjName() { return m_strObjName; }

	// 오브젝트의 위치 설정 및 반환
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }

	// 오브젝트의 크기 설정 및 반환
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	// 오브젝트의 방향 설정 및 반환
	void SetDir(Vec2 _vDir) { m_vDir = _vDir; _vDir.Normalize(); }
	Vec2 GetDir() { return m_vDir; }

	// 오브젝트의 생존 여부 반환
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
	virtual void FastUpdate() {}; 	// Scene이 시작되기 직전에 호출되는 함수
	virtual void Update() = 0;
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);

	void ComponentRender(HDC _dc);

	virtual CObject* Clone() = 0;
};