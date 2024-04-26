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
	// Ȱ��ȭ�� Scene�ȿ� UILayer��, (UINT)LAYER_TYPE::UI �����´�.
	CLayer* pUILayer = pCurScene->GetLayer((UINT)LAYER_TYPE::UI);
	// ���� ���ȿ� �ִ� UILayer���� UI��ҵ��� ��� ���͸� �����´�.
	vector<CObject*>& vecUI = pUILayer->GetUIGroup();
	// UI������ ���� �κ��� ����Ű�� �ݺ��ڸ� �����Ѵ�.
	vector<CObject*>::iterator iter = vecUI.begin();
	// UI���͸� ���ۺκк��� ���κб��� ��ȸ�ϸ�,���� ��Ŀ�̵� UI�� ��ġ�� ã�´�.
	for (; iter != vecUI.end(); ++iter)
	{
		// ���� ��ȸ���� UI��Ұ� ��Ŀ�̵� UI�� �������� Ȯ���Ѵ�.
		if (m_pFocusedUI == *iter)
		{
			break;
		}
	}

	// ���� ������ �� �ڷ� ���� ��ü
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

CUI* CUIManager::GetFocusedUI()
{
	// ���� Ȱ��ȭ�� Scene�� �����´�.
	CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();
	// Ȱ��ȭ�� Scene�ȿ� UILayer��, (UINT)LAYER_TYPE::UI �����´�.
	CLayer* pUILayer = pCurScene->GetLayer((UINT)LAYER_TYPE::UI);
	// ���� ���ȿ� �ִ� UILayer���� UI��ҵ��� ��� ���͸� �����´�.
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
	// 1. FocusedUI�� �ִ��� Ȯ��
	m_pFocusedUI = GetFocusedUI();

	// m_pFocusedUI�� nullptr�̸�, �ƹ��� �۾��� ���� �ʰ� ���� �ڵ� ������� �Ѿ��.
	if (nullptr == m_pFocusedUI)
	{
		return;
	}

	// 2. FocusedUI ������, �θ� UI�� �ڽ� UI �� �� ���� Ÿ���� �� UI�� �����´�.
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);
}
