#include "stdafx.h"
#include "First_Stage.h"

#include "Soul_Gage.h"
#include "HP_Stone.h"
#include "Player.h"
#include "Mantis_Spike.h"

#include "Abstract_Factory.h"
#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"

CFirst_Stage::CFirst_Stage()
{
}


CFirst_Stage::~CFirst_Stage()
{
	Release();
}

void CFirst_Stage::Initialize(void)
{
	// Rect
	CObj_Mgr::Get_Instance()->Load_Wall(L"../Data/First_Stage_Wall.dat");
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_SPIKE, CAbstract_Factory<CMantis_Spike>::Create());

	// BackGround
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/First_Stage.bmp", L"First_Stage");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/First_Stage_Front.bmp", L"First_Stage_Front");

	// Player
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstract_Factory<CPlayer>::Create(3930.f, 1010.f));

	// UI
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, CAbstract_Factory<CSoul_Gage>::Create());
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, CAbstract_Factory<CHP_Stone>::Create(240.f, 140.f));

	CScroll_Mgr::Get_Instance()->Initialize();
}

int CFirst_Stage::Update(void)
{
	CObj_Mgr::Get_Instance()->Update();

	return OBJ_NOEVENT;
}

void CFirst_Stage::Late_Update(void)
{
	CObj_Mgr::Get_Instance()->Late_Update();

	m_fPlayerX = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX;

	if (m_fPlayerX > 4000.f)
	{
		CScene_Mgr::Get_Instance()->Scene_Change(SC_START_STAGE);
		CScroll_Mgr::Get_Instance()->Initialize();
	}
	else if (m_fPlayerX < 0.f)
		CScene_Mgr::Get_Instance()->Scene_Change(SC_MANTIS_LORDS_STAGE);
}

void CFirst_Stage::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"First_Stage");
	HDC	hMemDC2 = CBmp_Mgr::Get_Instance()->Find_Img(L"First_Stage_Front");

	int	iScrollX = (int)CScroll_Mgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, iScrollX, iScrollY, FIRST_STAGE_CX, FIRST_STAGE_CY, hMemDC, 0, 0, SRCCOPY);
	CObj_Mgr::Get_Instance()->Render(hDC);
	GdiTransparentBlt(hDC,
		iScrollX,
		iScrollY,
		FIRST_STAGE_CX,
		FIRST_STAGE_CY,
		hMemDC2,
		0,
		0,
		FIRST_STAGE_CX,
		FIRST_STAGE_CY,
		RGB(3, 3, 3));
	for (auto& iter : *CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_UI))
		iter->Render(hDC);
}

void CFirst_Stage::Release(void)
{
	//CObj_Mgr::Get_Instance()->Release();
	CObj_Mgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->StopAll();
}
