#include "../pch.h"
#include "CScene.h"
#include "../Layer/CLayer.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	// 모든 Layer들을 메모리에서 삭제
	for (size_t i = 0; i < m_arrLayers.size(); ++i)
	{
		delete m_arrLayers[i];
	}
	m_arrLayers.clear();
}

void CScene::DeleteLayer(LAYER_TYPE _eTarget)
{
	for (size_t i = 0; i < m_arrLayers.size(); ++i)
	{
		if (static_cast<UINT>(m_arrLayers[i]->GetLayerType()) == static_cast<UINT>(_eTarget))
		{
			delete m_arrLayers[i];
			m_arrLayers.erase(m_arrLayers.begin() + i);
			break;
		}
	}
}

CLayer* CScene::CreateLayer(UINT _LayerType)
{
	CLayer* pLayer = new CLayer();
	pLayer->SetLayerType(static_cast<LAYER_TYPE>(_LayerType));
	m_arrLayers.push_back(pLayer);
	return pLayer;
}

void CScene::FastUpdate()
{
	for (size_t i = 0; i < m_arrLayers.size(); ++i)
	{
		m_arrLayers[i]->FastUpdate();
	}
}

void CScene::Update()
{
	for (size_t i = 0; i < m_arrLayers.size(); ++i)
	{
		if (!m_arrLayers[i]->IsActive())
		{
			m_arrLayers[i]->Update();
		}
	}
}

void CScene::FinalUpdate()
{
	for (size_t i = 0; i < m_arrLayers.size(); ++i)
	{
		m_arrLayers[i]->FinalUpdate();
	}
}

void CScene::Render(HDC _dc)
{
	for (size_t i = 0; i < m_arrLayers.size(); ++i)
	{
		m_arrLayers[i]->Render(_dc);
	}
}