#pragma once

#include "../pch.h"

class CLayer;

class CScene
{
public:
	friend class CLayer;

public:
	CScene();
	virtual ~CScene();

private:
	wstring				m_strSceneName;
	vector<CLayer*>		m_arrLayer[(UINT)LAYER_TYPE::END];

public:
	void SetSceneName(const wstring& _strSceneName) { m_strSceneName = _strSceneName; }
	const wstring& GetSceneName() const { return m_strSceneName; }

public:
	void AddLayer(CLayer* _pLayer, LAYER_TYPE _eType) { m_arrLayer[(UINT)_eType].push_back(_pLayer); }

	const vector<CLayer*>& GetGroupLayer(LAYER_TYPE	_eType) { return m_arrLayer[(UINT)_eType]; }
	void DeleteLayer(LAYER_TYPE _eTarget);
	void DeleteAll();

public:
	CLayer* FindLayer(const wstring& _strLayerName);
	CLayer* CreateLayer(const wstring& _strLayerName, int _iZOrder, LAYER_TYPE _eType);

public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void FastUpdate();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};

// ���� ����: ��ü ���� ����� �������� ū �������� ������, �� ������ �ٽ� �������� ���� ��ҳ� �������� �����ȴ�.
// ū Ʋ�δ� Stage1Ŭ����, Stage2Ŭ������ ����� �۾�.

// Layer����: �ϳ��� Scene���� ��� Layer, ������Ʈ Layer, ĳ���� Layer, UI Layer ���� ���� �� �ֽ��ϴ�.
// �̷��� Layer ������ ���� ������ �������� ȿ�������� ������ �� ������, Ư�� Layer���� ������� �� ó���� ���������ϴ�(��: ����� ������ �ְ� ĳ���͸� �̵�)