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
	// 관리하는 애니메이션 싹다 삭제.
	Safe_Delete_Map(m_mapAnim);
}

// 애니메이션 생성(애니메이션 이름, 택스쳐, 좌상단, 프레임 크기, , 재생시간, 프레임카운트)
void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, 
	Vec2 _vLT, Vec2 _vSlicesize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	// 이미 같은 이름의 애니메이션이 존재하는지 확인
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim);	// 중복이 없어야 한다.

	pAnim = new CAnimation();
	pAnim->SetAnimName(_strName);
	pAnim->m_pAnimator = this;	// 애니메이션 객체에 Animatior 포인터 설정
	// 아틀라스 애니메이션 생성
	pAnim->CreateAtlas(_pTex, _vLT, _vSlicesize, _vStep, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator::CreateFrameAnimation(const wstring& _strName, const vector<wstring>& _vecFileNames,
	Vec2 _vLT, Vec2 _vSlicesize, float _fDuration)
{
	// 이미 같은 이름의 애니메이션이 존재하는지 확인
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim); // 중복이 없어야 한다.

	pAnim = new CAnimation();
	pAnim->SetAnimName(_strName);
	pAnim->m_pAnimator = this;

	vector<CTexture*> vecTextures;

	// 각 파일 이름에 대해 텍스처를 로드하여 vecTextures에 추가
	for (const auto& fileName : _vecFileNames)
	{
		CTexture* pTexture = CResourceManager::GetInst()->LoadTexture(fileName, fileName);
		vecTextures.push_back(pTexture);
	}

	// 로드된 텍스처 벡터를 사용하여 프레임 애니메이션 생성
	pAnim->CreateFrame(vecTextures, _vLT, _vSlicesize, _fDuration);

	// 애니메이션 맵에 추가
	m_mapAnim.insert(make_pair(_strName, pAnim));
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	// map을 순회하면서 지정된 이름(_strName)에 해당하는 애니메이션을 찾는다.
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);

	// 찾은 결과가 맵의 끝을 가리키면 해당 애니메이션이 없으므로 nullptr를 반환
	if (iter == m_mapAnim.end())
		return nullptr;

	// 찾은 애니메이션의 포인터를 반환
	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	// 주어진 이름으로 애니메이션을 찾아 현재 애니메이션으로 설정합니다.
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _bRepeat;
}

void CAnimator::Pause(const wstring& _strName, bool _bPause)
{
	// 주어진 이름으로 애니메이션을 찾아 현재 애니메이션으로 설정합니다.
	m_pCurAnim = FindAnimation(_strName);

	// 현재 애니메이션이 존재하는 경우
	if (nullptr != m_pCurAnim)
	{
		// 해당 애니메이션이 일시 정지 여부를 설정합니다.
		m_pCurAnim->m_bPause = _bPause;
	}
}

void CAnimator::Update()
{
}

void CAnimator::FinalUpdate()
{
	// 현재 애니메이션이 존재하는 경우
	if (nullptr != m_pCurAnim)
	{
		// 현재 애니메이션을 업데이트 합니다.
		m_pCurAnim->Update();

		// 반복 재생이 있고, 애니메이션이 종료된 경우
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			// 애니메이션의 프레임을 처음으로 되돌립니다.
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::Render(HDC _dc)
{
	// 현재 애니메이션이 존재하는 경우
	if (nullptr != m_pCurAnim)
		m_pCurAnim->Render(_dc);	// 현재 애니메이션을 렌더링합니다.
}