#include "../pch.h"
#include "CUI.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CCamera.h"
#include "../Module/SelectGDI.h"
#include "../Scene/CScene.h"

CUI::CUI(bool _bCamAff) :
	m_vecChildUI{},
	m_pParentUI(nullptr),
	m_vFinalPos{},
	m_bCamAffected(_bCamAff),
	m_bMouseOn(false),
	m_bLbtnDown(false)
{
}

CUI::CUI(const CUI& _origin)	:
	CObject(_origin),
	m_pParentUI(nullptr),
	m_bCamAffected(_origin.m_bCamAffected),
	m_bMouseOn(false),
	m_bLbtnDown(false)
{
	// ���� ��ü�� ��� �ڽ� UI�� �����Ͽ� ���� ������ ��ü�� �߰�
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); ++i)
	{
		// _orgin�� �ڽ� UI�� �����ϰ�, ������ UI�� ���� ��ü�� �ڽ����� �߰�
		AddChild(_origin.m_vecChildUI[i]->Clone());
	}
}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}

CUI* CUI::GetFindChild(CUI* _parentUI, const wstring& _childUI)
{
	for (UINT i = 0; i < _parentUI->GetChildUI().size(); ++i)
	{
		if (_parentUI->GetChildUI()[i]->GetObjName() == _childUI)
		{
			if (_parentUI->GetChildUI()[i] == nullptr)
				assert(nullptr);

			return _parentUI->GetChildUI()[i];
		}
	}
	return nullptr;
}

void CUI::Update_Child()
{
	// �� ����� CUI�����͸� ����Ͽ�, ObjectŬ������ Update()�Լ��� ȣ���Ѵ�.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Update();
	}
}

void CUI::FinalUpdate_Child()
{
	// �� ����� CUI�����͸� ����Ͽ�, ObjectŬ������ FinalUpdate()�Լ��� ȣ���Ѵ�.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->FinalUpdate();
	}
}

void CUI::Render_Child(HDC _dc)
{
	// �� ����� CUI�����͸� ����Ͽ�, ObjectŬ������ Render()�Լ��� ȣ���Ѵ�.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render(_dc);
	}
}

void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;		// ���콺�� ���� ��ġ�� �����´�.
	Vec2 vUIScale = GetScale();		// UI�� ũ�⸦ �����´�.

	// ���� UI�� ī�޶� ������ �޴´ٸ�, ���콺�� ��ġ�� ���� ȭ�� ��ǥ�� ��ȯ�Ѵ�.
	if (m_bCamAffected)
	{
		CCamera::GetInst()->GetRealPos(vMousePos);
	}

	// ���콺�� UI���� �ȿ� �ִ��� ���θ� Ȯ���Ѵ�.
	if (m_vFinalPos.x <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + vUIScale.x
		&& m_vFinalPos.y <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vUIScale.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::MouseOn()
{
}

void CUI::MouseLBtnDown()
{
}

void CUI::MouseLBtnUp()
{
}

void CUI::MouseLBtnClicked()
{
}

void CUI::Update()
{
	Update_Child();
}

void CUI::FinalUpdate()
{
	// UI�� �ִϸ��̼��� ���� ���� �ֱ� ������, �θ��� FinalUpdate() ȣ��
	CObject::FinalUpdate();

	// UI�� ���� ��ǥ�� ���Ѵ�.
	m_vFinalPos = GetPos();

	if (GetParent())
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	// UI Mouseüũ
	MouseOnCheck();

	FinalUpdate_Child();
}

void CUI::Render(HDC _dc)
{
	// UI ����� ���� ��ġ�� ũ�⸦ �����´�.
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	// UI�� ī�޶� ������ �޴´ٸ�, �׿� �°� ��ġ�� �����Ѵ�.
	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}

	if (m_bLbtnDown)
	{
		SelectGDI select(_dc, PEN_TYPE::GREEN);

		Rectangle(_dc,
			int(vPos.x),
			int(vPos.y),
			int(vPos.x + vScale.x),
			int(vPos.y + vScale.y));
	}

	else
	{
		Rectangle(_dc,
			int(vPos.x),
			int(vPos.y),
			int(vPos.x + vScale.x),
			int(vPos.y + vScale.y));
	}

	Render_Child(_dc);
}