#include "pch.h"
#include "func.h"
#include "Manager/CEventManager.h"

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent evn = { };
	evn.eEven = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)_eNext;

	CEventManager::GetInst()->AddEvent(evn);
}