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
	wstring				m_strSceneName;				// Scene의 이름
	vector<CLayer*>		m_arrLayers;				// Layer을 저장하는 배열

public:
	// Scene의 이름을 설정(Set)과 반환(Get)하는 함수
	void SetSceneName(const wstring& _strSceneName) { m_strSceneName = _strSceneName; }
	const wstring& GetSceneName() const { return m_strSceneName; }

	// 특정 Layer 타입의 Layer을 반환하는 함수
	CLayer* CreateLayer(UINT _LayerType);
	// 특정 타입의 Layer을 삭제하는 함수
	void DeleteLayer(LAYER_TYPE _eTarget);


public:
	virtual void Enter() = 0;	// 해당 Scene에 진입 시 호출
	virtual void Exit() = 0;	// 해당 Scene에 탈출 시 호출

	virtual void FastUpdate();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};

// 세계 구성: 전체 게임 세계는 여러개의 큰 지역으로 나뉘고, 각 지역은 다시 여러개의 작은 장소나 던전으로 구성된다.
// 큰 틀로는 Stage1클래스, Stage2클래스로 나누어서 작업.

// Layer구성: 하나의 Scene에는 배경 Layer, 오브젝트 Layer, 캐릭터 Layer, UI Layer 등이 있을 수 있습니다.
// 이러한 Layer 구분을 통해 게임의 렌더링을 효율적으로 관리할 수 있으며, 특정 Layer만을 대상으로 한 처리가 가능해집니다(예: 배경은 정지해 있고 캐릭터만 이동)