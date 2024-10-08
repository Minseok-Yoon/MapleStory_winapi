#include "../pch.h"
#include "CItem.h"
#include "../Component/CCollider.h"
#include "../Module/SelectGDI.h"

CItem::CItem()
{
    AddCollider();   
    auto itemCollider = GetCollider().back();
    itemCollider->SetColTag("Item");
    itemCollider->SetOffsetPos(Vec2{ 0.f, 0.f });
    itemCollider->SetScale(Vec2{ 30.f, 30.f });
    itemCollider->SetLayer(1);
}

CItem::~CItem()
{
}

void CItem::Update()
{
}

void CItem::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

    // m_vPos는 절대 좌표로 유지
    Vec2 vPos = GetPos();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);

    // 아이템의 렌더링 로직을 여기에 추가할 수 있습니다.
    Ellipse(_dc, vPos.x - 10, vPos.y - 10, vPos.x + 10, vPos.y + 10);

    // Collider의 위치를 가져옴
    if (!GetCollider().empty())
    {
        auto itemCollider = GetCollider().back();
        Vec2 colPos = itemCollider->GetPos();
        Vec2 colRenderPos = CCamera::GetInst()->GetRenderPos(colPos);
    }
}