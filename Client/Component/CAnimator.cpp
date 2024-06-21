#include "../pch.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "../Manager/CResourceManager.h"

CAnimator::CAnimator()	:
	m_pCurAnim(nullptr),
	m_pOwner(nullptr),
	m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	// �����ϴ� �ִϸ��̼� �ϴ� ����.
	Safe_Delete_Map(m_mapAnim);
}

// �ִϸ��̼� ����(�ִϸ��̼� �̸�, �ý���, �»��, ������ ũ��, , ����ð�, ������ī��Ʈ)
void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, 
	Vec2 _vLT, Vec2 _vSlicesize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	// �̹� ���� �̸��� �ִϸ��̼��� �����ϴ��� Ȯ��
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim);	// �ߺ��� ����� �Ѵ�.

	pAnim = new CAnimation();
	pAnim->SetAnimName(_strName);
	pAnim->m_pAnimator = this;	// �ִϸ��̼� ��ü�� Animatior ������ ����
	// ��Ʋ�� �ִϸ��̼� ����
	pAnim->CreateAtlas(_pTex, _vLT, _vSlicesize, _vStep, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator::CreateFrameAnimation(const wstring& _strName, const vector<wstring>& _vecFileNames,
	Vec2 _vLT, Vec2 _vSlicesize, float _fDuration)
{
	// �̹� ���� �̸��� �ִϸ��̼��� �����ϴ��� Ȯ��
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim); // �ߺ��� ����� �Ѵ�.

	pAnim = new CAnimation();
	pAnim->SetAnimName(_strName);
	pAnim->m_pAnimator = this;

	vector<CTexture*> vecTextures;

	// �� ���� �̸��� ���� �ؽ�ó�� �ε��Ͽ� vecTextures�� �߰�
	for (const auto& fileName : _vecFileNames)
	{
		CTexture* pTexture = CResourceManager::GetInst()->LoadTexture(fileName, fileName);
		vecTextures.push_back(pTexture);
	}

	// �ε�� �ؽ�ó ���͸� ����Ͽ� ������ �ִϸ��̼� ����
	pAnim->CreateFrame(vecTextures, _vLT, _vSlicesize, _fDuration);

	// �ִϸ��̼� �ʿ� �߰�
	m_mapAnim.insert(make_pair(_strName, pAnim));
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	// map�� ��ȸ�ϸ鼭 ������ �̸�(_strName)�� �ش��ϴ� �ִϸ��̼��� ã�´�.
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);

	// ã�� ����� ���� ���� ����Ű�� �ش� �ִϸ��̼��� �����Ƿ� nullptr�� ��ȯ
	if (iter == m_mapAnim.end())
		return nullptr;

	// ã�� �ִϸ��̼��� �����͸� ��ȯ
	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	// �־��� �̸����� �ִϸ��̼��� ã�� ���� �ִϸ��̼����� �����մϴ�.
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _bRepeat;
}

void CAnimator::Pause(const wstring& _strName, bool _bPause)
{
	// �־��� �̸����� �ִϸ��̼��� ã�� ���� �ִϸ��̼����� �����մϴ�.
	m_pCurAnim = FindAnimation(_strName);

	// ���� �ִϸ��̼��� �����ϴ� ���
	if (nullptr != m_pCurAnim)
	{
		// �ش� �ִϸ��̼��� �Ͻ� ���� ���θ� �����մϴ�.
		m_pCurAnim->m_bPause = _bPause;
	}
}

void CAnimator::Update()
{
}

void CAnimator::FinalUpdate()
{
	// ���� �ִϸ��̼��� �����ϴ� ���
	if (nullptr != m_pCurAnim)
	{
		// ���� �ִϸ��̼��� ������Ʈ �մϴ�.
		m_pCurAnim->Update();

		// �ݺ� ����� �ְ�, �ִϸ��̼��� ����� ���
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			// �ִϸ��̼��� �������� ó������ �ǵ����ϴ�.
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::Render(HDC _dc)
{
	// ���� �ִϸ��̼��� �����ϴ� ���
	if (nullptr != m_pCurAnim)
		m_pCurAnim->Render(_dc);	// ���� �ִϸ��̼��� �������մϴ�.
}