#pragma once

class CObject;
class CTexture;

enum class CAM_EFFECT
{
	NONE,
	FADE_IN,
	FADE_OUT,
};

struct tCamEffect
{
	CAM_EFFECT		eEffect;		// 카메라 효과
	float			fDuration;		// 효과 최대 진행 시간
	float			fCurTime;		// 카메라 효과 현재 진행된 시간
};

typedef struct _tagResolution
{
	unsigned int	iW;	
	unsigned int	iH;

	_tagResolution() :
		iW(0),
		iH(0)
	{
	}

	_tagResolution(int x, int y) :
		iW(x),
		iH(y)
	{
	}
}RESOLUTION, * PRESOLUTION;

class CCamera
{
	SINGLETON(CCamera);

private:
	Vec2		m_vLookAt;				// 카메라가 보는 위치
	Vec2		m_vCurLookAt;			// 이전위치와 현재위치 보정 위치
	Vec2		m_vPrevLookAt;			// 카메라가 보는 이전의 프레임 위치

	CObject*	m_pTargetObj;			// 카메라 타겟 오브젝트
	Vec2		m_vDiff;				// 해상도 중심위치와, 카메라 LookAt간의 차이값

	float		m_fGetTargetTime;		// 타겟을 따라가는데 걸리는 시간
	float		m_fGetTargetSpeed;		// 타겟을 따라가는 속도
	float		m_fAccTime;				// 누적시간

	list<tCamEffect> m_listCamEffect;	// 순차적으로 효과를 처리하기 위해 vector말고 list로 사용
	CTexture* m_pVeilTex;				// 카메라 가림막 택스처(검은색으로)

	RESOLUTION	m_tClientRS;
	RESOLUTION	m_tWorldRS;

public:
	void SetLookAt(Vec2 _vLook)
	{
		m_vLookAt = _vLook;
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
		m_fGetTargetSpeed = fMoveDist / m_fGetTargetTime;
		m_fAccTime = 0.f;
	}

	Vec2 GetLookAt() { return m_vCurLookAt; }

	void SetTarget(CObject* _pTarget) { m_pTargetObj = _pTarget; }
	CObject* GetTarget() { return m_pTargetObj; }

	Vec2 GetRenderPos(Vec2 _vObjPos) { return (_vObjPos - m_vDiff); }
	Vec2 GetRealPos(Vec2 _vRenderPos) { return (_vRenderPos + m_vDiff); }

	void FadeIn(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_IN;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
			assert(nullptr);
	}

	void FadeOut(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_OUT;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
			assert(nullptr);
	}

private:
	void CalDiff();		// 화면 중앙좌표와 LookAt 좌표간 차이값 계산

public:
	void Init(const RESOLUTION& tRS, const RESOLUTION& tWorldRS);
	void Update();
	void Render(HDC _dc);
};