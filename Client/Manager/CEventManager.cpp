#include "../pch.h"
#include "CEventManager.h"
#include "../Object/CObject.h"
#include "../Manager/CSceneManager.h"

CEventManager::CEventManager()
{
}

CEventManager::~CEventManager()
{
}

void CEventManager::Update()
{
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
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next Scene Type
		// Scene º¯°æ
		CSceneManager::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);
	}
	break;
	}
}