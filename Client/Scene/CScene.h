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
	wstring				m_strSceneName;				// Scene�� �̸�
	vector<CLayer*>		m_arrLayers;				// Layer�� �����ϴ� �迭

public:
	// Scene�� �̸��� ����(Set)�� ��ȯ(Get)�ϴ� �Լ�
	void SetSceneName(const wstring& _strSceneName) { m_strSceneName = _strSceneName; }
	const wstring& GetSceneName() const { return m_strSceneName; }

	// Ư�� Layer Ÿ���� Layer�� ��ȯ�ϴ� �Լ�
	CLayer* CreateLayer(UINT _LayerType);
	// Ư�� Ÿ���� Layer�� �����ϴ� �Լ�
	void DeleteLayer(LAYER_TYPE _eTarget);


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