#include "stdafx.h"
#include "Start_Stage.h"

#include "Soul_Gage.h"
#include "HP_Stone.h"
#include "Player.h"

#include "Abstract_Factory.h"
#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"


CStart_Stage::CStart_Stage()
{
}


CStart_Stage::~CStart_Stage()
{
	Release();
}

void CStart_Stage::Initialize(void)
{
	// Rect
	CObj_Mgr::Get_Instance()->Load_Wall(L"../Data/Start_Stage_Wall.dat");

	// BackGround
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/Start_Stage.bmp", L"Start_Stage");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/Start_Stage_Front.bmp", L"Start_Stage_Front");

	// Player
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstract_Factory<CPlayer>::Create(1838.f, 1015.f));

	// UI
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, CAbstract_Factory<CSoul_Gage>::Create());
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, CAbstract_Factory<CHP_Stone>::Create(240.f, 140.f));

	CScroll_Mgr::Get_Instance()->Initialize();
}

int CStart_Stage::Update(void)
{
	CObj_Mgr::Get_Instance()->Update();

	return OBJ_NOEVENT;
}

void CStart_Stage::Late_Update(void)
{
	CObj_Mgr::Get_Instance()->Late_Update();

	m_fPlayerX = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX;

	if (m_fPlayerX < 825.f)
		CScene_Mgr::Get_Instance()->Scene_Change(SC_FIRST_STAGE);

}

void CStart_Stage::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"Start_Stage");
	HDC	hMemDC2 = CBmp_Mgr::Get_Instance()->Find_Img(L"Start_Stage_Front");

	int	iScrollX = (int)CScroll_Mgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, iScrollX, iScrollY, START_STAGE_CX, START_STAGE_CY, hMemDC, 0, 0, SRCCOPY);
	CObj_Mgr::Get_Instance()->Render(hDC);
	GdiTransparentBlt(hDC,
		iScrollX,
		iScrollY,
		START_STAGE_CX,
		START_STAGE_CY,
		hMemDC2,
		0,
		0,
		START_STAGE_CX,
		START_STAGE_CY,
		RGB(3, 3, 3));
	for (auto& iter : *CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_UI))
		iter->Render(hDC);
}

void CStart_Stage::Release(void)
{
	//CObj_Mgr::Get_Instance()->Release();
	CObj_Mgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->StopAll();
}
