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
	// �̹� ��Ŀ�� ���� UI�ų�, ��Ŀ���� ������û�ϴ� ���
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		// ���� �� ��� ��� �ش���� ������ ���ο� UI�� ��Ŀ���� �����ϰ� �Լ��� �����Ѵ�.
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	// ���� Ȱ��ȭ�� Scene�� �����´�.
	CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();
	// ���� ������ UI��ҵ��� ��� ���͸� �����´�.
	vector<CObject*>& vecUI = pCurScene->GetUIGroup(OBJECT_TYPE::UI);

	// UI������ ���� �κ��� ����Ű�� �ݺ��ڸ� �����Ѵ�.
	vector<CObject*>::iterator iter = vecUI.begin();
	// UI���͸� ���ۺκк��� ���κб��� ��ȸ�ϸ�,���� ��Ŀ�̵� UI�� ��ġ�� ã�´�.
	for (; iter != vecUI.end(); ++iter)
	{
		// ���� ��ȸ���� UI��Ұ� ��Ŀ�̵� UI�� �������� Ȯ���Ѵ�.
		if (m_pFocusedUI == *iter)
			break;
	}

	// ���� ������ �� �ڷ� ���� ��ü
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

CUI* CUIManager::GetFocusedUI()
{
	// ���� Ȱ��ȭ�� Scene�� �����´�.
	CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();
	// ���� ���ȿ� �ִ� UILayer���� UI��ҵ��� ��� ���͸� �����´�.
	vector<CObject*>& vecUI = pCurScene->GetUIGroup(OBJECT_TYPE::UI);

	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	// ���� ��Ŀ�� UI�� �޾Ƶΰ� ����Ǿ����� Ȯ���Ѵ�.
	CUI* pFocusedUI = m_pFocusedUI;

	if (!bLbtnTap)
	{
		return pFocusedUI;
	}

	// ���콺 ���ʹ�ư TAP�� �߻��ߴٴ� ����
	vector<CObject*>::iterator targetIter = vecUI.end();
	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetIter = iter;
		}
	}

	// �̹��� Focus�� UI�� ����
	if (vecUI.end() == targetIter)
	{
		return nullptr;
	}

	pFocusedUI = (CUI*)*targetIter;

	// ���� ������ �ǵڷ� ���� ��ü
	vecUI.erase(targetIter);
	vecUI.push_back(pFocusedUI);
	return pFocusedUI;
}

CUI* CUIManager::GetTargetedUI(CUI* _pParentUI)
{
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// 1. �θ� UI���� ��� �ڽĵ��� �˻��Ѵ�.
	CUI* pTargetUI = nullptr;

	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;

	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);

	while (!queue.empty())
	{
		// ť���� ������ �ϳ� ������
		CUI* pUI = queue.front();
		queue.pop_front();

		// ť���� ������ UI�� TargetUI���� Ȯ��
		// Ÿ�� UI�� ��, �� �켱������ ���� ������ �� ���� ������ �ڽ� UI
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

	// ���ʹ�ư ����, ������ üũ�� �ٽ� �����Ѵ�.
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
	// 1. FocusedUI�� �ִ��� Ȯ��
	m_pFocusedUI = GetFocusedUI();

	// m_pFocusedUI�� nullptr�̸�, �ƹ��� �۾��� ���� �ʰ� ���� �ڵ� ������� �Ѿ��.
	if (nullptr == m_pFocusedUI)
	{
		return;
	}

	// 2. FocusedUI ������, �θ� UI�� �ڽ� UI �� �� ���� Ÿ���� �� UI�� �����´�.
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

			// ���ʹ�ư ����, ���ȴ� üũ�� �ٽ� �����Ѵ�.
			pTargetUI->m_bLbtnDown = false;
		}
	}
}