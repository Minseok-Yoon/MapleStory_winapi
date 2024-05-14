#include "../pch.h"
#include "CUIManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "../Scene/CScene.h"
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
	// 현재 씬에서 UI요소들이 담긴 벡터를 가져온다.
	vector<CObject*>& vecUI = pCurScene->GetUIGroup(OBJECT_TYPE::UI);

	// UI벡터의 시작 부분을 가리키는 반복자를 생성한다.
	vector<CObject*>::iterator iter = vecUI.begin();
	// UI벡터를 시작부분부터 끝부분까지 순회하며,현재 포커싱된 UI의 위치를 찾는다.
	for (; iter != vecUI.end(); ++iter)
	{
		// 현재 순회중인 UI요소가 포커싱된 UI와 동일한지 확인한다.
		if (m_pFocusedUI == *iter)
			break;
	}

	// 벡터 내에서 맨 뒤로 순번 교체
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

CUI* CUIManager::GetFocusedUI()
{
	// 현재 활성화된 Scene을 가져온다.
	CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();
	// 현재 씬안에 있는 UILayer에서 UI요소들이 담긴 벡터를 가져온다.
	vector<CObject*>& vecUI = pCurScene->GetUIGroup(OBJECT_TYPE::UI);

	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	// 기존 포커싱 UI를 받아두고 변경되었는지 확인한다.
	CUI* pFocusedUI = m_pFocusedUI;

	if (!bLbtnTap)
	{
		return pFocusedUI;
	}

	// 마우스 왼쪽버튼 TAP이 발생했다는 전제
	vector<CObject*>::iterator targetIter = vecUI.end();
	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetIter = iter;
		}
	}

	// 이번에 Focus된 UI가 없다
	if (vecUI.end() == targetIter)
	{
		return nullptr;
	}

	pFocusedUI = (CUI*)*targetIter;

	// 백터 내에서 맨뒤로 순번 교체
	vecUI.erase(targetIter);
	vecUI.push_back(pFocusedUI);
	return pFocusedUI;
}

CUI* CUIManager::GetTargetedUI(CUI* _pParentUI)
{
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// 1. 부모 UI포함 모든 자식들을 검사한다.
	CUI* pTargetUI = nullptr;

	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;

	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);

	while (!queue.empty())
	{
		// 큐에서 데이터 하나 꺼내기
		CUI* pUI = queue.front();
		queue.pop_front();

		// 큐에서 꺼내온 UI가 TargetUI인지 확인
		// 타겟 UI들 중, 더 우선순위가 높은 기준은 더 낮은 계층의 자식 UI
		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)
			{
				vecNoneTarget.push_back(pTargetUI);
			}

			pTargetUI = pUI;
		}
		else
		{
			vecNoneTarget.push_back(pUI);
		}

		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}

	// 왼쪽버튼 떼면, 눌렀던 체크를 다시 해제한다.
	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); ++i)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
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

	bool bLbtnAway = KEY_AWAY(KEY::LBTN);
	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	if (nullptr != pTargetUI)
	{
		pTargetUI->MouseOn();

		if (bLbtnTap)
		{
			pTargetUI->MouseLBtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			pTargetUI->MouseLBtnUp();

			if (pTargetUI->m_bLbtnDown)
			{
				pTargetUI->MouseLBtnClicked();
			}

			// 왼쪽버튼 떼면, 눌렸던 체크를 다시 해제한다.
			pTargetUI->m_bLbtnDown = false;
		}
	}
}