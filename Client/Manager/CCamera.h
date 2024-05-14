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
	CAM_EFFECT		eEffect;		// ī�޶� ȿ��
	float			fDuration;		// ȿ�� �ִ� ���� �ð�
	float			fCurTime;		// ī�޶� ȿ�� ���� ����� �ð�
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
	Vec2		m_vLookAt;				// ī�޶� ���� ��ġ
	Vec2		m_vCurLookAt;			// ������ġ�� ������ġ ���� ��ġ
	Vec2		m_vPrevLookAt;			// ī�޶� ���� ������ ������ ��ġ

	CObject*	m_pTargetObj;			// ī�޶� Ÿ�� ������Ʈ
	Vec2		m_vDiff;				// �ػ� �߽���ġ��, ī�޶� LookAt���� ���̰�

	float		m_fGetTargetTime;		// Ÿ���� ���󰡴µ� �ɸ��� �ð�
	float		m_fGetTargetSpeed;		// Ÿ���� ���󰡴� �ӵ�
	float		m_fAccTime;				// �����ð�

	list<tCamEffect> m_listCamEffect;	// ���������� ȿ���� ó���ϱ� ���� vector���� list�� ���
	CTexture* m_pVeilTex;				// ī�޶� ������ �ý�ó(����������)

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
	void CalDiff();		// ȭ�� �߾���ǥ�� LookAt ��ǥ�� ���̰� ���

public:
	void Init(const RESOLUTION& tRS, const RESOLUTION& tWorldRS);
	void Update();
	void Render(HDC _dc);
};