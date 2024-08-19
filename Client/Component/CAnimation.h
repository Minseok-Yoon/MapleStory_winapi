#pragma once

class CAnimator;
class CTexture;

struct tAnimFrm
{
	Vec2		vLT;		// 애니메이션 텍스처에서의 시작지점
	Vec2		vSlice;		// 프레임의 크기
	Vec2		vOffSet;	// 프레임의 오프셋(프레임의 상대적인 위치를 조정)
	float		fDuration;	// 프레임의 지속시간(해당 프레임이 얼마 동안 화면에 표시되는지 결정)
	CTexture*	pTexture;
};

class CAnimation
{
	friend class CAnimator;

public:
	CAnimation();
	~CAnimation();

private:
	wstring				m_strAnimName;	
	vector<tAnimFrm>	m_vecFrm;		// 모든 프레임 정보
	int					m_iCurFrm;		// 현재 프레임
	float				m_fAccTime;		// 시간 누적
	bool				m_bFinish;		// 재생 끝에 도달여부
	bool				m_bPause;		// 일시정지 여부
	bool				m_bEnd;			// 종료 여부

	CAnimator*			m_pAnimator;	// 애니메이션의 제어
	CTexture*			m_pTex;			// 애니메이션이 사용할 택스처

public:
	const wstring& GetAnimName() { return m_strAnimName; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }

	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0.f;
	}
	tAnimFrm& GetFrame(int _iIdx) { return m_vecFrm[_iIdx]; }

	bool IsFinish() { return m_bFinish; }
	bool IsPause() { return m_bPause; }
	bool IsEnd() { return m_bEnd; }

	CAnimator* GetAnimator() { return m_pAnimator; }

private:
	void SetAnimName(const wstring& _strAnimName) { m_strAnimName = _strAnimName; }

public:
	void CreateAtlas(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	void CreateFrame(const vector<CTexture*>& _vecTextures, Vec2 _vLT, Vec2 _vSliceSize, float _fDuration);
	void ResetTime();

	void Update();
	void Render(HDC _dc);
};