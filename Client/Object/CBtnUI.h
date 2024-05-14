#pragma once
#include "CUI.h"
#include "../Scene/CScene.h"
#include "CObject.h"

// 'BTN_FUNC'는 함수 포인터 타입을 정의하는 typedef
typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR);
// 'SCENE_MEMFUNC'는 멤버 함수 포인터 타입을 정의하는 typedef
typedef void(CScene::*SCENE_MEMFUNC) (void);
typedef void(CObject::*OBJECT_MEMFUNC) (void);

class CBtnUI :
    public CUI
{
public:
    CBtnUI();
    virtual ~CBtnUI();

private:
    BTN_FUNC        m_pFunc;
    DWORD_PTR       m_pParam1;
    DWORD_PTR       m_pParam2;

    SCENE_MEMFUNC   m_pSceneFunc;
    CScene*         m_pSceneInst;

    OBJECT_MEMFUNC  m_pObjectFunc;
    CObject*        m_pObjectInst;

public:
    virtual void MouseOn();
    virtual void MouseLBtnDown();
    virtual void MouseLBtnUp();
    virtual void MouseLBtnClicked();

    void SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2)
    {
        m_pFunc = _pFunc;
        m_pParam1 = _param1;
        m_pParam2 = _param2;
    }

    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc);
    void SetClickedCallBack(CObject* _pObject, OBJECT_MEMFUNC _pObjectFunc);

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CBtnUI);
};