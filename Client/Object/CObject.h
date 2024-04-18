#pragma once

class CObject
{
public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

private:
	wstring		m_strObjName;

	Vec2		m_vPos;
	Vec2		m_vScale;

	bool		m_bAlive;

public:
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }

	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	bool IsDead() { return !m_bAlive; }

private:
	void SetDead() { m_bAlive = false; }

public:
	virtual void FastUpdate() {}; 	// Scene이 시작되기 직전에 호출되는 함수
	virtual void Update() = 0;
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};