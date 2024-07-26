#pragma once

class CObject;

class CGravity
{
	friend class CObject;

public:
	CGravity();
	~CGravity();

private:
	CObject*	m_pOwner;
	bool		m_bGround;

public:
	void SetOnGround(bool _bGround);

	bool IsOnGround() const { return m_bGround; }

public:
	void FinalUpdate();
};