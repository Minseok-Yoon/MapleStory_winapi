#pragma once

// Singletion Macro
#define SINGLETON(type)	public:\
							static type* GetInst()\
							{\
								static type Manager;\
								return &Manager;\
							}\
						private:\
							type();\
							~type();

#define SAFE_RELEASE(p)		if(p) { p->Release(); p = NULL; }

#define fDeltaTime CTimeManager::GetInst()->GetfDeltaTime()
#define DeltaTime CTimeManager::GetInst()->GetDeltaTime()

// 암시적 복사생성자 호출할 경우 부모 복사생성자 호출된다.
#define CLONE(type) type* Clone() { return new type(*this); }

#define KEY_CHECK(key, state) CKeyManager::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS CKeyManager::GetInst()->GetMousePos()

enum class SCENE_TYPE
{
	START,
	STAGE_01,
	STAGE_02,

	END,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	BLACK,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END,
};

enum class OBJECT_TYPE
{
	DEFAULT,
	BACKGROUND,
	PIXEL_BACKGROUND,
	PLAYER,
	MONSTER,
	RECTANGLE,
	PIXEL,

	UI = 31,
	END = 32,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,

	END,
};

enum class MON_STATE
{
	IDLE,		// 대기 상태
	PATROL,		// 순찰 상태
	TRACE,		// 추적 상태
	ATTACK,		// 공격
	RUN,
	DEAD,
};

enum class PLAYER_STATE
{
	IDLE,
	WALK,
	ATTACK,
	JUMP,
	DEAD,
};

enum COLLISION_STATE
{
	CS_ENTER,
	CS_STAY,
	CS_EXIT
};

enum COLLIDER_TYPE
{
	CT_RECTANGLE,
	CT_PIXEL
};