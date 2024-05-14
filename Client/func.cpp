#include "pch.h"
#include "func.h"
#include "Manager/CEventManager.h"

void CreateObject(CObject* _pObj, OBJECT_TYPE _eObject)
{
	// 이벤트 객체를 생성한다.
	tEvent evn = { };
	evn.eEven = EVENT_TYPE::CREATE_OBJECT;	// 오브젝트 생성 이벤트 타입을 설정
	evn.lParam = (DWORD_PTR)_pObj;			// 새로 생성될 객체의 포인터를 lParam에 저장.
	evn.wParam = (DWORD_PTR)_eObject;		// 객체가 속할 오브젝트 타입을 wParam에 저장

	// EventManager에 이벤트를 추가한다.
	CEventManager::GetInst()->AddEvent(evn);
}

void DeleteObject(CObject* _pObj)
{
	// 이벤트 객체를 생성한다.
	tEvent evn = { };
	evn.eEven = EVENT_TYPE::DELETE_OBJECT;	// 삭제 이벤트 타입을 설정한다.
	evn.lParam = (DWORD_PTR)_pObj;			// 삭제될 객체의 포인터를 lParam에 저장

	// EventManager에 이벤트를 추가한다.
	CEventManager::GetInst()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	// 이벤트 객체를 생성한다.
	tEvent evn = { };
	evn.eEven = EVENT_TYPE::SCENE_CHANGE;	// 씬 변경 이벤트 타입을 설정한다.
	evn.lParam = (DWORD_PTR)_eNext;			// 변경될 다음 씬의 타입을 lParam에 저장한다

	// EventManager에 이벤트를 추가한다.
	CEventManager::GetInst()->AddEvent(evn);
}

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	// 이벤트 객체를 생성한다.
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)_pAI;
	evn.wParam = (DWORD_PTR)_eNextState;

	CEventManager::GetInst()->AddEvent(evn);
}
