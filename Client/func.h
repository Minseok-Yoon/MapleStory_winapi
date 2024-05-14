#pragma once

class CObject;
class AI;

void CreateObject(CObject* _pObj, OBJECT_TYPE _eObject);
void DeleteObject(CObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext);
void ChangeAIState(AI* _pAI, MON_STATE _eNextState);

// 벡터 내부의 메모리를 삭제하는 함수 템플릿
template<typename T>
void Safe_Delete_Vec(vector<T>& _vec)
{
	// 벡터 내의 모든 요소를 반복하여 확인
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		// 요소가 nullptr이 아니라면 메모리 해제
		if (nullptr != _vec[i])
			delete _vec[i];
	}
	// 요소를 삭제한 후 벡터를 비워줌
	_vec.clear();
}

// Map 내부를 순회하면서 메모리를 삭제하는 함수 템플릿
template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	// 맵의 반복자 선언 및 초기화
	typename map<T1, T2>::iterator iter = _map.begin();

	// 맵의 모든 요소를 반복하여 확인
	for (; iter != _map.end(); ++iter)
	{
		// 요소가 nullptr이 아니라면 메모리 해제
		if (nullptr != iter->second)
			delete iter->second;
	}
	// 요소를 삭제한 후 맵을 비워줌
	_map.clear();
}