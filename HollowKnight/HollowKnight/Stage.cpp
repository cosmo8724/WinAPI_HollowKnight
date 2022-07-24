#include "stdafx.h"
#include "Stage.h"

#include "Soul_Gage.h"
#include "HP_Stone.h"
#include "Player.h"
#include "Mantis_Lord.h"

#include "Abstract_Factory.h"
#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"

CStage::CStage()
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize(void)
{
	CObj_Mgr::Get_Instance()->Load_Wall(L"../Data/Mantis_Lords_Stage_Wall.dat");

	// Player
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstract_Factory<CPlayer>::Create(3800.f, 6400.f));

	// UI
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, CAbstract_Factory<CSoul_Gage>::Create());
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, CAbstract_Factory<CHP_Stone>::Create(240.f, 140.f));	

	// Mantis_Lords
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstract_Factory<CMantis_Lord>::Create());
	dynamic_cast<CMantis_Lord*>(CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_MONSTER)->back())->Set_Identify(MIDDLE_LORD);
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstract_Factory<CMantis_Lord>::Create());
	dynamic_cast<CMantis_Lord*>(CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_MONSTER)->back())->Set_Identify(LEFT_LORD);
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstract_Factory<CMantis_Lord>::Create());
	dynamic_cast<CMantis_Lord*>(CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_MONSTER)->back())->Set_Identify(RIGHT_LORD);


	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/Mantis_Lords_Stage.bmp", L"Mantis_Lords_Stage");
	//CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Mantis_Lord_Ground_Front.bmp", L"Mantis_Lord_Ground_Front");

	CScroll_Mgr::Get_Instance()->Initialize();
}

int CStage::Update(void)
{
	CObj_Mgr::Get_Instance()->Update();

	return 0;
}

void CStage::Late_Update(void)
{
	CObj_Mgr::Get_Instance()->Late_Update();

	float	fPlayerX = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX;
	float	fPlayerY = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fY;

	if(fPlayerY < 6150.f)
		CScene_Mgr::Get_Instance()->Scene_Change(SC_FIRST_STAGE);

	if(fPlayerX < 200.f)
		CScene_Mgr::Get_Instance()->Scene_Change(SC_THE_RADIANCE_STAGE);
}

void CStage::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"Mantis_Lords_Stage");
	//HDC	hMemDC2 = CBmp_Mgr::Get_Instance()->Find_Img(L"Mantis_Lord_Ground_Front");

	int	iScrollX = (int)CScroll_Mgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, iScrollX, iScrollY, MANTIS_LORDS_STAGE_CX, MANTIS_LORDS_STAGE_CY, hMemDC, 0, 0, SRCCOPY);
	CObj_Mgr::Get_Instance()->Render(hDC);
	/*GdiTransparentBlt(hDC,
		iScrollX,
		iScrollY,
		2369,
		800,
		hMemDC2,
		0,
		0,
		2369,
		800,
		RGB(3, 3, 3));*/
	for (auto& iter : *CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_UI))
		iter->Render(hDC);
}

void CStage::Release(void)
{
	//CObj_Mgr::Get_Instance()->Release();
	CObj_Mgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->StopAll();
}
