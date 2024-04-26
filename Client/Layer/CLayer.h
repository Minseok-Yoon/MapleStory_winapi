#pragma once

class CObject;

class CLayer
{
public:
	CLayer();
	~CLayer();

private:
	UINT				m_LayerType;
	int					m_iZOrder;
	bool				m_bIsActive;
	wstring				m_strLayerName;
	vector<CObject*>	m_arrObj[(UINT)OBJECT_TYPE::END];

public:
	UINT GetLayerType() const { return m_LayerType; }

	void SetiZOrder(int _iZOrder) { m_iZOrder = _iZOrder; }
	int GetiZOrder() const { return m_iZOrder; }

	void SetLayerName(const wstring& _strLayerName) { m_strLayerName = _strLayerName; }
	const wstring& GetLayerName() { return m_strLayerName; }

public:
	void AddObject(CObject* _pObj, OBJECT_TYPE _eType) { m_arrObj[(UINT)_eType].push_back(_pObj); }

	const vector<CObject*>& GetGroupObject(OBJECT_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(OBJECT_TYPE _eTarget);
	void DeleteAll();

	bool IsActive() { return !m_bIsActive; }

	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)OBJECT_TYPE::UI]; }

public:
	static bool LayerSort(CLayer* _pL1, CLayer* _pL2);

	virtual void FastUpdate();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};