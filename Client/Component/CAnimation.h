#pragma once

class CAnimator;
class CTexture;

struct tAnimFrm
{
	Vec2	vLT;		// �ִϸ��̼� �ؽ�ó������ ��������
	Vec2	vSlice;		// �������� ũ��
	Vec2	vOffSet;	// �������� ������(�������� ������� ��ġ�� ����)
	float	fDuration;	// �������� ���ӽð�(�ش� �������� �� ���� ȭ�鿡 ǥ�õǴ��� ����)
};

class CAnimation
{
	friend class CAnimator;

public:
	CAnimation();
	~CAnimation();

private:
	wstring				m_strAnimName;	
	CAnimator*			m_pAnimator;	// �ִϸ��̼��� ����
	CTexture*			m_pTex;			// �ִϸ��̼��� ����� �ý�ó
	vector<tAnimFrm>	m_vecFrm;		// ��� ������ ����
	int					m_iCurFrm;		// ���� ������
	float				m_fAccTime;		// �ð� ����
	bool				m_bFinish;		// ��� ���� ���޿���
	bool				m_bPause;		// �Ͻ����� ����

public:
	const wstring& GetAnimName() { return m_strAnimName; }
	CAnimator* GetAnimator() { return m_pAnimator; }
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

private:
	void SetAnimName(const wstring& _strAnimName) { m_strAnimName = _strAnimName; }

public:
	void Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);

	void Update();
	void Render(HDC _dc);
};

