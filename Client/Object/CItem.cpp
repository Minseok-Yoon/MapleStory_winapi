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

    // m_vPos�� ���� ��ǥ�� ����
    Vec2 vPos = GetPos();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);

    // �������� ������ ������ ���⿡ �߰��� �� �ֽ��ϴ�.
    Ellipse(_dc, vPos.x - 10, vPos.y - 10, vPos.x + 10, vPos.y + 10);

    // Collider�� ��ġ�� ������
    if (!GetCollider().empty())
    {
        auto itemCollider = GetCollider().back();
        Vec2 colPos = itemCollider->GetPos();
        Vec2 colRenderPos = CCamera::GetInst()->GetRenderPos(colPos);

        // Collider ��ġ ����� ���
        wchar_t debugText[256];
        swprintf_s(debugText, L"Collider Pos: (%f, %f)\n", colRenderPos.x, colRenderPos.y);
        TextOut(_dc, 500, 10, debugText, wcslen(debugText));
    }

    // Ellipse ��ġ ����� ���
    wchar_t debugText2[256];
    swprintf_s(debugText2, L"Ellipse Pos: (%f, %f)\n", vPos.x, vPos.y);
    TextOut(_dc, 500, 30, debugText2, wcslen(debugText2));
}