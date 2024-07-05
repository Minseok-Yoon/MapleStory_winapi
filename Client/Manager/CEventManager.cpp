#include "../pch.h"
#include "CEventManager.h"
#include "../Object/CObject.h"
#include "../Scene/CScene.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CUIManager.h"
#include "../Module/AI.h"
#include "../Module/CState.h"

CEventManager::CEventManager()
{
}

CEventManager::~CEventManager()
{
}

void CEventManager::Update()
{
	// 이전 프레임에서 등록해둔 Dead Object들을 삭제한다.
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		if (m_vecDead[i])
		{
			delete m_vecDead[i];
			m_vecDead[i] = nullptr;
		}
	}
	m_vecDead.clear();

	// Event 처리
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Excute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

void CEventManager::Excute(const tEvent& _eve)
{
	switch (_eve.eEven)
	{
		case EVENT_TYPE::CREATE_OBJECT:
		{
			// lParam : Object Adress
			// wParam : Group Type
			CObject* pNewObj = (CObject*)_eve.lParam;
			OBJECT_TYPE eType = (OBJECT_TYPE)_eve.wParam;

			CSceneManager::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
		}
			break;

		case EVENT_TYPE::DELETE_OBJECT:
		{
			// lParam : Object Adress
			// Object를 Dead 상태로 변경
			// 삭제예정. 오브젝트를 모아둔다.
			CObject* pDeadObj = (CObject*)_eve.lParam;
			
			if (!pDeadObj->IsDead())
			{
				pDeadObj->SetDead();
				m_vecDead.push_back(pDeadObj);
			}
		}
			break;

		case EVENT_TYPE::SCENE_CHANGE:
		{
			// lParam : Next Scene Type
			// Scene 변경
			CSceneManager::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);

			CUIManager::GetInst()->SetFocusedUI(nullptr);
		}
			break;

		case EVENT_TYPE::CHANGE_AI_STATE:
		{
			// lParam : AI
			// wParam : Next Type
			AI* pAI = (AI*)_eve.lParam;
			MON_STATE eNextState = (MON_STATE)_eve.wParam;
			pAI->ChangeState(eNextState);
		}
			break;
	}
}