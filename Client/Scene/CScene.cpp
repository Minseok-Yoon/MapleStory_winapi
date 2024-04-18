#include "../pch.h"
#include "CScene.h"
#include "../Layer/CLayer.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrLayer[i].size(); ++j)
		{
			if (nullptr != m_arrLayer[i][j])
				delete m_arrLayer;
		}
	}
}

void CScene::DeleteLayer(LAYER_TYPE _eTarget)
{
	Safe_Delete_Vec(m_arrLayer[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		DeleteLayer((LAYER_TYPE)i);
	}
}

CLayer* CScene::FindLayer(const wstring& _strLayerName)
{
	for (size_t i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrLayer[i].size(); ++j)
		{
			if (m_arrLayer[i][j]->GetLayerName() == _strLayerName)
				return m_arrLayer[i][j];
		}
	}
	return nullptr;
}

CLayer* CScene::CreateLayer(const wstring& _strLayerName, int _iZOrder, LAYER_TYPE _eType)
{
	CLayer* pLayer = new CLayer;

	pLayer->SetLayerName(_strLayerName);
	pLayer->SetiZOrder(_iZOrder);

	m_arrLayer[(UINT)_eType].push_back(pLayer);

	sort(m_arrLayer[(UINT)_eType].begin(), m_arrLayer[(UINT)_eType].end(), CLayer::LayerSort);

	return pLayer;
}

void CScene::FastUpdate()
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrLayer[i].size(); ++j)
		{
			m_arrLayer[i][j]->FastUpdate();
		}
	}
}

void CScene::Update()
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrLayer[i].size(); ++j)
		{
			if (!m_arrLayer[i][j]->IsActive())
			{
				m_arrLayer[i][j]->Update();
			}
		}
	}
}

void CScene::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrLayer[i].size(); ++j)
		{
			m_arrLayer[i][j]->FinalUpdate();
		}
	}
}

void CScene::Render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		vector<CLayer*>::iterator iter = m_arrLayer[i].begin();

		for (; iter != m_arrLayer[i].end();)
		{
			if (!(*iter)->IsActive())
			{
				(*iter)->Render(_dc);
				++iter;
			}
			else
			{
				iter = m_arrLayer[i].erase(iter);
			}
		}
	}
}
