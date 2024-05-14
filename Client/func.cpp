#include "pch.h"
#include "func.h"
#include "Manager/CEventManager.h"

void CreateObject(CObject* _pObj, OBJECT_TYPE _eObject)
{
	// �̺�Ʈ ��ü�� �����Ѵ�.
	tEvent evn = { };
	evn.eEven = EVENT_TYPE::CREATE_OBJECT;	// ������Ʈ ���� �̺�Ʈ Ÿ���� ����
	evn.lParam = (DWORD_PTR)_pObj;			// ���� ������ ��ü�� �����͸� lParam�� ����.
	evn.wParam = (DWORD_PTR)_eObject;		// ��ü�� ���� ������Ʈ Ÿ���� wParam�� ����

	// EventManager�� �̺�Ʈ�� �߰��Ѵ�.
	CEventManager::GetInst()->AddEvent(evn);
}

void DeleteObject(CObject* _pObj)
{
	// �̺�Ʈ ��ü�� �����Ѵ�.
	tEvent evn = { };
	evn.eEven = EVENT_TYPE::DELETE_OBJECT;	// ���� �̺�Ʈ Ÿ���� �����Ѵ�.
	evn.lParam = (DWORD_PTR)_pObj;			// ������ ��ü�� �����͸� lParam�� ����

	// EventManager�� �̺�Ʈ�� �߰��Ѵ�.
	CEventManager::GetInst()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	// �̺�Ʈ ��ü�� �����Ѵ�.
	tEvent evn = { };
	evn.eEven = EVENT_TYPE::SCENE_CHANGE;	// �� ���� �̺�Ʈ Ÿ���� �����Ѵ�.
	evn.lParam = (DWORD_PTR)_eNext;			// ����� ���� ���� Ÿ���� lParam�� �����Ѵ�

	// EventManager�� �̺�Ʈ�� �߰��Ѵ�.
	CEventManager::GetInst()->AddEvent(evn);
}

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	// �̺�Ʈ ��ü�� �����Ѵ�.
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)_pAI;
	evn.wParam = (DWORD_PTR)_eNextState;

	CEventManager::GetInst()->AddEvent(evn);
}
