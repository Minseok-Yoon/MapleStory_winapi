#pragma once
#include "../pch.h"
#include "../Factory/CMonFactory.h"

class CObject;

class CScene
{
public:
	CScene();
	virtual ~CScene();

private:
	wstring				m_strSceneName;						// Scene�� �̸�
	
	Vec2				m_vMapSize;
	CObject*			m_pPlayer;

public:
	vector<CObject*>	m_arrObj[(UINT)OBJECT_TYPE::END];	// ������Ʈ�� ���� �� ������ ���͸� �׷� ���� ��ŭ ����

public:
	// Scene�� �̸��� ����(Set)�� ��ȯ(Get)�ϴ� �Լ�
	void SetSceneName(const wstring& _strSceneName) { m_strSceneName = _strSceneName; }
	const wstring& GetSceneName() const { return m_strSceneName; }

	void SetMapSize(Vec2 _vMapSize) { m_vMapSize = _vMapSize; }
	Vec2 GetMapSize() { return m_vMapSize; }

	CObject* GetPlayer() { return m_pPlayer; }

public:
	void AddObject(CObject* _pObj, OBJECT_TYPE _eType) 
	{
		m_arrObj[(UINT)_eType].push_back(_pObj); 
	}
	void DeleteGroup(OBJECT_TYPE _eTarget);
	void DeleteAll();

	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }
	CObject* GetCurPlayer() { return m_pPlayer; }

	const vector<CObject*>& GetGroupObject(OBJECT_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	vector<CObject*>& GetUIGroup(const OBJECT_TYPE& _eType) { return m_arrObj[(UINT)OBJECT_TYPE::UI]; }

public:
	virtual void Enter() = 0;	// �ش� Scene�� ���� �� ȣ��
	virtual void Exit() = 0;	// �ش� Scene�� Ż�� �� ȣ��

	virtual void FastUpdate();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};

// ���� ����: ��ü ���� ����� �������� ū �������� ������, �� ������ �ٽ� �������� ���� ��ҳ� �������� �����ȴ�.
// ū Ʋ�δ� Stage1Ŭ����, Stage2Ŭ������ ����� �۾�.

// Layer����: �ϳ��� Scene���� ��� Layer, ������Ʈ Layer, ĳ���� Layer, UI Layer ���� ���� �� �ֽ��ϴ�.
// �̷��� Layer ������ ���� ������ �������� ȿ�������� ������ �� ������, Ư�� Layer���� ������� �� ó���� ���������ϴ�(��: ����� ������ �ְ� ĳ���͸� �̵�)