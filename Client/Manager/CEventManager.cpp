#include "../pch.h"
#include "CEventManager.h"
#include "../Object/CObject.h"
#include "../Scene/CScene.h"
#include "../Layer/CLayer.h"
#include "../Manager/CSceneManager.h"

CEventManager::CEventManager()
{
}

CEventManager::~CEventManager()
{
}

void CEventManager::Update()
{
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();

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

		CSceneManager::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::END)->AddObject(pNewObj, eType);
	}
	break;

	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : Object Adress
		// Object를 Dead 상태로 변경
		// 삭제예정. 오브젝트를 모아둔다.
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();
		m_vecDead.push_back(pDeadObj);
	}
	break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next Scene Type
		// Scene 변경
		CSceneManager::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);
	}
	break;
	}
}