#pragma once

class CScene;

class CSceneManager
{
	friend class CEventManager;

	SINGLETON(CSceneManager);

private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];	// 모든 씬 목록
	CScene* m_pCurScene;						// 현재 씬

public:
	// 현재 씬을 반환하는 함수
	CScene* GetCurScene() { return m_pCurScene; }

private:
	// 씬을 변경하는 함수
	void ChangeScene(SCENE_TYPE _eNextScene);

public:
	void Init();
	void Update();
	void Render(HDC _dc);
};