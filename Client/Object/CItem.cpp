#include "../pch.h"
#include "CItem.h"
#include "../Component/CCollider.h"
#include "../Module/SelectGDI.h"

CItem::CItem()
{
    AddCollider();
    if (!GetCollider().empty())
    {
        auto itemCollider = GetCollider().back();
        itemCollider->SetColTag("Item");
        itemCollider->SetOffsetPos(Vec2{ -10.f, 0.f });
        itemCollider->SetScale(Vec2{ 200.f, 200.f });
        itemCollider->SetLayer(1);
    }
    else
    {
        OutputDebugStringA("Failed to add collider in CItem constructor.\n");
    }
}

CItem::~CItem()
{
}

void CItem::SetItemInfo(const Vec2& _pos)
{
	m_vPos = _pos;
}

void CItem::Update()
{
}

void CItem::FinalUpdate()
{
}

void CItem::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

    Vec2 vPos = GetItemInfo();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);

    // 아이템의 렌더링 로직을 여기에 추가할 수 있습니다.
    Ellipse(_dc, vPos.x - 10, vPos.y - 10, vPos.x + 10, vPos.y + 10);
}