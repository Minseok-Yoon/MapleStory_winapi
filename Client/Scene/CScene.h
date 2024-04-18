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

// 세계 구성: 전체 게임 세계는 여러개의 큰 지역으로 나뉘고, 각 지역은 다시 여러개의 작은 장소나 던전으로 구성된다.
// 큰 틀로는 Stage1클래스, Stage2클래스로 나누어서 작업.

// Layer구성: 하나의 Scene에는 배경 Layer, 오브젝트 Layer, 캐릭터 Layer, UI Layer 등이 있을 수 있습니다.
// 이러한 Layer 구분을 통해 게임의 렌더링을 효율적으로 관리할 수 있으며, 특정 Layer만을 대상으로 한 처리가 가능해집니다(예: 배경은 정지해 있고 캐릭터만 이동)