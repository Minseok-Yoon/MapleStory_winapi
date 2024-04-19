#pragma once

class CScene;

class CSceneManager
{
	friend class CEventManager;

	SINGLETON(CSceneManager);

private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];
	CScene* m_pCurScene;

public:
	CScene* GetCurScene() { return m_pCurScene; }

private:
	void ChangeScene(SCENE_TYPE _eNextScene);

public:
	void Init();
	void Update();
	void Render(HDC _dc);
};