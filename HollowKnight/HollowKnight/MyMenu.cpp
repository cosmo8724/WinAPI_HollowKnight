#include "stdafx.h"
#include "MyMenu.h"

#include "MyButton.h"
#include "MyPointer.h"

#include "Abstract_Factory.h"
#include "Bmp_Mgr.h"
#include "Obj_Mgr.h"
#include "Scene_Mgr.h"
#include "Key_Mgr.h"
#include "SoundMgr.h"


CMyMenu::CMyMenu()
{
}


CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize(void)
{
	// BackGround
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Menu/MainMenu.bmp", L"MainMenu");

	// Button
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Menu/Button_Start.bmp", L"Button_Start");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Menu/Button_Exit.bmp", L"Button_Exit");

	CObj*	pObj = CAbstract_Factory<CMyButton>::Create(800.f, 718.f);
	pObj->Set_FrameKey(L"Button_Start");
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);

	pObj = CAbstract_Factory<CMyButton>::Create(800.f, 784.5f);
	pObj->Set_FrameKey(L"Button_Exit");
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);

	CMyPointer* pPointer = dynamic_cast<CMyPointer*>(CAbstract_Factory<CMyPointer>::Create(800, 718.f));
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, pPointer);

	CSoundMgr::Get_Instance()->PlaySound(L"Title_OST.wav", SOUND_BGM, 0.3f);
}

int CMyMenu::Update(void)
{
	Key_Input();
	CKey_Mgr::Get_Instance()->Key_Reset();
	CObj_Mgr::Get_Instance()->Update();

	return OBJ_NOEVENT;
}

void CMyMenu::Late_Update(void)
{
	CObj_Mgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"MainMenu");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObj_Mgr::Get_Instance()->Render(hDC);
}

void CMyMenu::Release(void)
{
	CObj_Mgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->StopAll();
}

void CMyMenu::Key_Input(void)
{
	if (CKey_Mgr::Get_Instance()->Key_Down(VK_UP))
	{
		for (auto& iter : *CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_UI))
		{
			iter->Set_Pos(800.f, 718.f);
			dynamic_cast<CMyPointer*>(iter)->Set_FrameStart(0);
		}
	}
	else if (CKey_Mgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		for (auto& iter : *CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_UI))
		{
			iter->Set_Pos(800.f, 784.5f);
			dynamic_cast<CMyPointer*>(iter)->Set_FrameStart(0);
		}
	}

	if (CKey_Mgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CObj*	pObj = CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_UI)->back();

		if (pObj->Get_Info().fY == 718.f)
			CScene_Mgr::Get_Instance()->Scene_Change(SC_START_STAGE);
		else
			PostQuitMessage(0);
	}
}
