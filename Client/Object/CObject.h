#pragma once
#include "../Manager/CCamera.h"

class CCollider;

class CObject
{
	friend class CEventManager;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

private:
	wstring		m_strObjName;		// Object의 이름
	Vec2		m_vPos;				// Object의 위치
	Vec2		m_vScale;			// Object의 크기

	Vec2		m_vDir;

	bool		m_bAlive;

	// Component
	CCollider* m_pCollider;

public:
	void SetObjName(const wstring& _strObjName) { m_strObjName = _strObjName; }
	const wstring& GetObjName() { return m_strObjName; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }

	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	void SetDir(Vec2 _vDir) { m_vDir = _vDir; _vDir.Normalize(); }
	Vec2 GetDir() { return m_vDir; }

	bool IsDead() { return !m_bAlive; }

	CCollider* GetCollider() { return m_pCollider; }

private:
	void SetDead() { m_bAlive = false; }

public:
	void CreateCollider();

public:
	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

public:
	virtual void FastUpdate() {}; 	// Scene이 시작되기 직전에 호출되는 함수
	virtual void Update() = 0;
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);

	void ComponentRender(HDC _dc);

	virtual CObject* Clone() = 0;
};