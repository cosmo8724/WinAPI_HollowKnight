#include "stdafx.h"
#include "MainGame.h"

#include "Obj_Mgr.h"
#include "Line_Mgr.h"
#include "Key_Mgr.h"
#include "Scroll_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scene_Mgr.h"
#include "SoundMgr.h"


CMainGame::CMainGame()
	: m_dwTime(GetTickCount()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Back.bmp", L"Back");
	CSoundMgr::Get_Instance()->Initialize();
	CScene_Mgr::Get_Instance()->Scene_Change(SC_MENU);
	//CLine_Mgr::Get_Instance()->Initialize();
}

void CMainGame::Update()
{
	if (CKey_Mgr::Get_Instance()->Key_Down(VK_F9))
	{
		if (m_bVisibleHitBox)
			m_bVisibleHitBox = false;
		else
			m_bVisibleHitBox = true;
	}

	CScene_Mgr::Get_Instance()->Update();
	//CLine_Mgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CScene_Mgr::Get_Instance()->Late_Update();
	CScroll_Mgr::Get_Instance()->Scroll_Lock();
}

void CMainGame::Render()
{
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	//if (SC_LOGO == CScene_Mgr::Get_Instance()->Get_SceneID())
	//	return;

	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"Back");
	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CScene_Mgr::Get_Instance()->Render(hMemDC);
}

void CMainGame::Release()
{
	CObj_Mgr::Get_Instance()->Destroy_Instance();
	CLine_Mgr::Get_Instance()->Destroy_Instance();
	CKey_Mgr::Get_Instance()->Destroy_Instance();
	CScroll_Mgr::Get_Instance()->Destroy_Instance();
	CScene_Mgr::Get_Instance()->Destroy_Instance();
	CBmp_Mgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->Destroy_Instance();

	ReleaseDC(g_hWnd, m_hDC);
}
