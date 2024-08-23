#include "../pch.h"
#include "CKeyManager.h"
#include "../Core/CCore.h"

int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT, //LEFT,
	VK_RIGHT,//RIGHT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,
	VK_F1,
};

CKeyManager::CKeyManager()
{
}

CKeyManager::~CKeyManager()
{
}

void CKeyManager::Init()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}

void CKeyManager::Update()
{
	// ������ ��Ŀ��(������ â�� Ȱ��ȭ, �� �������� �ִ� ����, �ּ�ȭ �Ǹ� ��Ȱ��ȭ)
	// ������ ��Ŀ�� �˾Ƴ���
	HWND hWnd = GetFocus();

	// ������ ��Ŀ�� ���� ��, Ű �̺�Ʈ ����
	if (nullptr != hWnd) 
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// Ű�� �����ִ�.(����Ű�� ����)
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000) 
			{
				if (m_vecKey[i].bPrevPush)
				{
					// �������� �����־���.
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					// ������ �������� �ʾҴ�.
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}
			// Ű�� �ȴ����ִ�.
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					// ������ �����־���.
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// �������� �ȴ����־���.
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPush = false;
			}
		}

		// Mouse ��ġ ���
		POINT ptPos = {};
		GetCursorPos(&ptPos);

		// ������ ��ǥ->Ŭ���̾�Ʈ ��ǥ(���ڷ� ���� ������ �ڵ鰪 �ִ´�.)
		ScreenToClient(CCore::GetInst()->GetMainHWnd(), &ptPos);

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}

	// ������ ��Ŀ�� ���� ����
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;

			if (m_vecKey[i].eState == KEY_STATE::TAP || m_vecKey[i].eState == KEY_STATE::HOLD)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			else if (m_vecKey[i].eState == KEY_STATE::AWAY)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}
}