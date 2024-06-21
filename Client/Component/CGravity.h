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

	bool		m_bFalling;
	bool		m_bPhysics;
	float		m_fGravityTime;
	int         m_iFallingCount;

public:
	static bool	m_onLand;

public:
	void SetOnGround(bool _bGround);

	void SetPhysics(bool _bPhysics) { m_bPhysics = _bPhysics; }
	bool GetPhysics() const { return m_bPhysics; }

	void ClearGravity() { m_bPhysics = 0.f; }

public:
	void FinalUpdate();
};