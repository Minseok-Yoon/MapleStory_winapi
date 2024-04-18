#include "../pch.h"
#include "../Scene/CScene.h"
#include "CLayer.h"
#include "../Object/CObject.h"

CLayer::CLayer() :
	m_bIsActive(true),
	m_iZOrder(0)
{
}

CLayer::~CLayer()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// m_arrObj[i] 그룹 벡터의 j 물체 삭제
			if (nullptr != m_arrObj[i][j])
				delete m_arrObj[i][j];
		}
	}
}

void CLayer::DeleteGroup(OBJECT_TYPE _eTarget)
{
	Safe_Delete_Vec(m_arrObj[(UINT)_eTarget]);
}

void CLayer::DeleteAll()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		DeleteGroup((OBJECT_TYPE)i);
	}
}

bool CLayer::LayerSort(CLayer* _pL1, CLayer* _pL2)
{
	return _pL1->GetiZOrder() < _pL2->GetiZOrder();
}

void CLayer::FastUpdate()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->FastUpdate();
		}
	}
}

void CLayer::Update()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->IsDead())
			{
				m_arrObj[i][j]->Update();
			}
		}
	}
}

void CLayer::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->FinalUpdate();
		}
	}
}

void CLayer::Render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		vector<CObject*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->Render(_dc);
				++iter;
			}
			else
			{
				iter = m_arrObj[i].erase(iter);
			}
		}
	}
}