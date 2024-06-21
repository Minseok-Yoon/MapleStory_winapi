#pragma once

class CAnimation;
class CObject;
class CTexture;

class CAnimator
{
	friend class CObject;

public:
	CAnimator();
	~CAnimator();

private:
	map<wstring, CAnimation*>	m_mapAnim;	// ��� Animation
	CAnimation*					m_pCurAnim;	// ���� ������� Animation
	CObject*					m_pOwner;	// Animator ���� ������Ʈ
	bool						m_bRepeat;	// �ݺ���� ����

public:
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSlicesize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	void CreateFrameAnimation(const wstring& _strName, const vector<wstring>& _vecFileNames, Vec2 _vLT, Vec2 _vSlicesize, float _fDuration);

	CAnimation* FindAnimation(const wstring& _strName);

	void Play(const wstring& _strName, bool _bRepeat);
	void Pause(const wstring& _strName, bool _bPause);

	void Update();
	void FinalUpdate();
	void Render(HDC _dc);
};