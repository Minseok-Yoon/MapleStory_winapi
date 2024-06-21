#pragma once

class CObject;
class AI;

void CreateObject(CObject* _pObj, OBJECT_TYPE _eObject);
void DeleteObject(CObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext);
void ChangeAIState(AI* _pAI, MON_STATE _eNextState);

// ���� ������ �޸𸮸� �����ϴ� �Լ� ���ø�
template<typename T>
void Safe_Delete_VecList(T& _p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = _p.end();

	for (iter = _p.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)) { delete (*iter); (*iter) = 0; }
	}

	_p.clear();
}

// Map ���θ� ��ȸ�ϸ鼭 �޸𸮸� �����ϴ� �Լ� ���ø�
template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	// ���� �ݺ��� ���� �� �ʱ�ȭ
	typename map<T1, T2>::iterator iter = _map.begin();

	// ���� ��� ��Ҹ� �ݺ��Ͽ� Ȯ��
	for (; iter != _map.end(); ++iter)
	{
		// ��Ұ� nullptr�� �ƴ϶�� �޸� ����
		if (nullptr != iter->second)
			delete iter->second;
	}
	// ��Ҹ� ������ �� ���� �����
	_map.clear();
}