#pragma once

class CObject
{
	friend class CEventManager;

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
	void SetObjName(const wstring& _strObjName) { m_strObjName = _strObjName; }
	const wstring& GetObjName() { return m_strObjName; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }

	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	bool IsDead() { return !m_bAlive; }

private:
	void SetDead() { m_bAlive = false; }

public:
	template<typename T>
	static T* AddObj(const wstring& _strObjName, class CLayer* pLayer = NULL)
	{
		T* pObj = new T;
		pObj->SetObjName(_strObjName);

		return pObj;
	}

public:
	virtual void FastUpdate() {}; 	// Scene�� ���۵Ǳ� ������ ȣ��Ǵ� �Լ�
	virtual void Update() = 0;
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);

	virtual CObject* Clone() = 0;
};