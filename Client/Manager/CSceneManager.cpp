#include "../pch.h"
#include "CSceneManager.h"
#include "../Scene/CScene_Start.h"
#include "../Scene/CScene_Stage01.h"
#include "../Scene/CScene_Stage02.h"
#include "../Manager/CEventManager.h"

CSceneManager::CSceneManager() :
	m_arrScene{},
	m_pCurScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	// 씬 전부 삭제
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		if (nullptr != m_arrScene[i])
		{
			delete m_arrScene[i];
		}
	}
}

void CSceneManager::ChangeScene(SCENE_TYPE _eNextScene)
{
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)_eNextScene];
	m_pCurScene->Enter();
}

void CSceneManager::Init()
{
	//Scene 생성
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetSceneName(L"Start Scene");

	//Scene_Stage01 생성
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CScene_Stage01;
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01]->SetSceneName(L"Scene Stage01");

	// 현재 씬을 START씬으로 지정하고, 현재 씬에 진입한다.
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START];
	m_pCurScene->Enter();
}

void CSceneManager::Update()
{
	m_pCurScene->Update();
	m_pCurScene->FinalUpdate();
}

void CSceneManager::Render(HDC _dc)
{
	m_pCurScene->Render(_dc);
}