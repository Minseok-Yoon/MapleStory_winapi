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

enum class LAYER_TYPE
{
	DEFAULT,
	BACKGROUND,
	OBJECT,
	PLAYER,
	MONSTER,
	UI,

	END = 10,
};

enum class OBJECT_TYPE
{
	DEFAULT,

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