#include "stdafx.h"
#include "The_Radiance_Stage.h"

#include "Player.h"
#include "The_Radiance.h"
#include "Soul_Gage.h"
#include "HP_Stone.h"

#include "Abstract_Factory.h"
#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"


CThe_Radiance_Stage::CThe_Radiance_Stage()
{
}

CThe_Radiance_Stage::~CThe_Radiance_Stage()
{
	Release();
}

void CThe_Radiance_Stage::Initialize(void)
{
	m_tRect1 = { 0, 3100, 3000, 4948 };
	m_tRect2 = { 852, 4730, 2147, 5000 };
	m_tRect3 = { 0, 4430, 3000, 4987 };

	// Rect
	CObj_Mgr::Get_Instance()->Load_Wall(L"../Data/The_Radiance_Stage_Wall.dat");

	// BackGround
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/The_Radiance_Stage.bmp", L"The_Radiance_Stage");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/The_Radiance_Stage_Rander1.bmp", L"The_Radiance_Stage_Rander1");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/The_Radiance_Stage_Rander2.bmp", L"The_Radiance_Stage_Rander2");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Map/The_Radiance_Stage_Rander3.bmp", L"The_Radiance_Stage_Rander3");

	// Player
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstract_Factory<CPlayer>::Create(1900.f, 4100.f));

	// UI
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, CAbstract_Factory<CSoul_Gage>::Create());
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_UI, CAbstract_Factory<CHP_Stone>::Create(240.f, 140.f));

	// Monster
	CObj_Mgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstract_Factory<CThe_Radiance>::Create());
}

int CThe_Radiance_Stage::Update(void)
{
	CObj_Mgr::Get_Instance()->Update();

	return OBJ_NOEVENT;
}

void CThe_Radiance_Stage::Late_Update(void)
{
	CObj_Mgr::Get_Instance()->Late_Update();
}

void CThe_Radiance_Stage::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"The_Radiance_Stage");
	HDC hMemDC2 = CBmp_Mgr::Get_Instance()->Find_Img(L"The_Radiance_Stage_Rander1");
	HDC hMemDC3 = CBmp_Mgr::Get_Instance()->Find_Img(L"The_Radiance_Stage_Rander2");
	HDC hMemDC4 = CBmp_Mgr::Get_Instance()->Find_Img(L"The_Radiance_Stage_Rander3");

	int	iScrollX = (int)CScroll_Mgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, iScrollX, iScrollY, THE_RADIANCE_STAGE_CX, THE_RADIANCE_STAGE_CY, hMemDC, 0, 0, SRCCOPY);

	CObj_Mgr::Get_Instance()->Render(hDC);
}

void CThe_Radiance_Stage::Release(void)
{
	CObj_Mgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->StopAll();
}
