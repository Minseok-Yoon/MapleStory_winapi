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
	// 원본 객체의 모든 자식 UI를 복제하여 복사 생성된 객체에 추가
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); ++i)
	{
		// _orgin의 자식 UI를 복제하고, 복제된 UI를 현재 객체의 자식으로 추가
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
	// 각 요소인 CUI포인터를 사용하여, Object클래스의 Update()함수를 호출한다.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Update();
	}
}

void CUI::FinalUpdate_Child()
{
	// 각 요소인 CUI포인터를 사용하여, Object클래스의 FinalUpdate()함수를 호출한다.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->FinalUpdate();
	}
}

void CUI::Render_Child(HDC _dc)
{
	// 각 요소인 CUI포인터를 사용하여, Object클래스의 Render()함수를 호출한다.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render(_dc);
	}
}

void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;		// 마우스의 현재 위치를 가져온다.
	Vec2 vUIScale = GetScale();		// UI의 크기를 가져온다.

	// 만약 UI가 카메라에 영향을 받는다면, 마우스의 위치를 실제 화면 좌표로 변환한다.
	if (m_bCamAffected)
	{
		CCamera::GetInst()->GetRealPos(vMousePos);
	}

	// 마우스가 UI영역 안에 있는지 여부를 확인한다.
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
	// UI가 애니메이션을 가질 수도 있기 때문에, 부모의 FinalUpdate() 호출
	CObject::FinalUpdate();

	// UI의 최종 좌표를 구한다.
	m_vFinalPos = GetPos();

	if (GetParent())
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	// UI Mouse체크
	MouseOnCheck();

	FinalUpdate_Child();
}

void CUI::Render(HDC _dc)
{
	// UI 요소의 최종 위치와 크기를 가져온다.
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	// UI가 카메라에 영향을 받는다면, 그에 맞게 위치를 조정한다.
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