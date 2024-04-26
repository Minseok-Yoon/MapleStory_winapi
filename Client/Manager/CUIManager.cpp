#include "../pch.h"
#include "CUIManager.h"
#include "CSceneManager.h"
#include "../Scene/CScene.h"
#include "../Layer/CLayer.h"
#include "../Object/CObject.h"
#include "../Object/CUI.h"

CUIManager::CUIManager()	:
	m_pFocusedUI(nullptr)
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::SetFocusedUI(CUI* _pUI)
{
	// 이미 포커싱 중인 UI거나, 포커싱을 해제요청하는 경우
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		// 위의 두 경우 모두 해당되지 않으면 새로운 UI에 포커싱을 설정하고 함수를 종료한다.
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	// 현재 활성화된 Scene을 가져온다.
	CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();
	// 활성화된 Scene안에 UILayer을, (UINT)LAYER_TYPE::UI 가져온다.
	CLayer* pUILayer = pCurScene->GetLayer((UINT)LAYER_TYPE::UI);
	// 현재 씬안에 있는 UILayer에서 UI요소들이 담긴 벡터를 가져온다.
	vector<CObject*>& vecUI = pUILayer->GetUIGroup();
	// UI벡터의 시작 부분을 가리키는 반복자를 생성한다.
	vector<CObject*>::iterator iter = vecUI.begin();
	// UI벡터를 시작부분부터 끝부분까지 순회하며,현재 포커싱된 UI의 위치를 찾는다.
	for (; iter != vecUI.end(); ++iter)
	{
		// 현재 순회중인 UI요소가 포커싱된 UI와 동일한지 확인한다.
		if (m_pFocusedUI == *iter)
		{
			break;
		}
	}

	// 벡터 내에서 맨 뒤로 순번 교체
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

CUI* CUIManager::GetFocusedUI()
{
	// 현재 활성화된 Scene을 가져온다.
	CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();
	// 활성화된 Scene안에 UILayer을, (UINT)LAYER_TYPE::UI 가져온다.
	CLayer* pUILayer = pCurScene->GetLayer((UINT)LAYER_TYPE::UI);
	// 현재 씬안에 있는 UILayer에서 UI요소들이 담긴 벡터를 가져온다.
	vector<CObject*>& vecUI = pUILayer->GetUIGroup();

	vecUI.push_back(m_pFocusedUI);
	return m_pFocusedUI;
}

CUI* CUIManager::GetTargetedUI(CUI* _pParentUI)
{
	return nullptr;
}

void CUIManager::Update()
{
	// 1. FocusedUI가 있는지 확인
	m_pFocusedUI = GetFocusedUI();

	// m_pFocusedUI가 nullptr이면, 아무런 작업도 하지 않고 다음 코드 블록으로 넘어간다.
	if (nullptr == m_pFocusedUI)
	{
		return;
	}

	// 2. FocusedUI 내에서, 부모 UI와 자식 UI 둘 중 실제 타겟팅 된 UI를 가져온다.
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);
}
