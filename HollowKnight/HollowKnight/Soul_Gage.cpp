#include "stdafx.h"
#include "Soul_Gage.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"

CSoul_Gage::CSoul_Gage()
{
}


CSoul_Gage::~CSoul_Gage()
{
}

void CSoul_Gage::Initialize(void)
{
	m_tInfo = { 190.f, 160.f, 190.f, 115.f };

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_Gage.bmp", L"Soul_Gage");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_0.bmp", L"Soul_Gage_0");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_10.bmp", L"Soul_Gage_10");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_20.bmp", L"Soul_Gage_20");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_30.bmp", L"Soul_Gage_30");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_40.bmp", L"Soul_Gage_40");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_50.bmp", L"Soul_Gage_50");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_60.bmp", L"Soul_Gage_60");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_70.bmp", L"Soul_Gage_70");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_80.bmp", L"Soul_Gage_80");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/Soul_90.bmp", L"Soul_Gage_90");
		
	m_pFrameKey = L"Soul_Gage";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 40;
	m_tFrame.dwFrameTime = GetTickCount();

	Update_Rect();
	m_tSoulRect.left = m_tRect.left + 2.5f;
	m_tSoulRect.top = m_tRect.top + 18;

	m_tSoulRect.pFrameKey = L"Soul_Gage_90";
	m_tSoulRect.tFrame.iFrameStart = 0;
	m_tSoulRect.tFrame.iFrameEnd = 0;
	m_tSoulRect.tFrame.iMotion = 0;
	m_tSoulRect.tFrame.dwFrameSpeed = 40;
	m_tSoulRect.tFrame.dwFrameTime = GetTickCount();

	m_iPlayerCurSoul = CObj_Mgr::Get_Instance()->Get_Player()->Get_Stat().iSoul;
	m_iPlayerLastSoul = m_iPlayerCurSoul;
}

int CSoul_Gage::Update(void)
{
	Update_State();
	Motion_Change();
	Move_Frame();
	Move_Soul_Frame();
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CSoul_Gage::Late_Update(void)
{
}

void CSoul_Gage::Render(HDC hDC)
{
	HDC hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(m_pFrameKey);
	HDC	hMemDC2 = CBmp_Mgr::Get_Instance()->Find_Img(m_tSoulRect.pFrameKey);

	int iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		(int)m_tInfo.fCX * m_tFrame.iFrameStart,
		(int)m_tInfo.fCY * m_tFrame.iMotion,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(11, 11, 11));

	GdiTransparentBlt(hDC,
		m_tSoulRect.left,
		m_tSoulRect.top,
		100,
		100,
		hMemDC2,
		100 * m_tSoulRect.tFrame.iFrameStart,
		100 * m_tSoulRect.tFrame.iMotion,
		100,
		100,
		RGB(11, 11, 11));

	if (m_bVisibleHitBox)
	{
		HPEN myPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		HPEN oldPen = (HPEN)::SelectObject(hDC, (HGDIOBJ)myPen);
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		Rectangle(hDC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
		SelectObject(hDC, oldPen);
		DeleteObject(myPen);
	}
}

void CSoul_Gage::Release(void)
{
}

void CSoul_Gage::Update_HitBox()
{
	m_tHitBox = m_tRect;
}

void CSoul_Gage::Update_State()
{
	m_iPlayerCurSoul = CObj_Mgr::Get_Instance()->Get_Player()->Get_Stat().iSoul;

	switch (m_iPlayerCurSoul)
	{
	case 90:
		m_tSoulRect.pFrameKey = L"Soul_Gage_90";
		m_tSoulRect.eSoulState = SOUL_90;
		break;

	case 80:
		m_tSoulRect.pFrameKey = L"Soul_Gage_80";
		m_tSoulRect.eSoulState = SOUL_80;
		break;

	case 70:
		m_tSoulRect.pFrameKey = L"Soul_Gage_70";
		m_tSoulRect.eSoulState = SOUL_70;
		break;

	case 60:
		m_tSoulRect.pFrameKey = L"Soul_Gage_60";
		m_tSoulRect.eSoulState = SOUL_60;
		break;

	case 50:
		m_tSoulRect.pFrameKey = L"Soul_Gage_50";
		m_tSoulRect.eSoulState = SOUL_50;
		break;

	case 40:
		m_tSoulRect.pFrameKey = L"Soul_Gage_40";
		m_tSoulRect.eSoulState = SOUL_40;
		break;

	case 30:
		m_tSoulRect.pFrameKey = L"Soul_Gage_30";
		m_tSoulRect.eSoulState = SOUL_30;
		break;

	case 20:
		m_tSoulRect.pFrameKey = L"Soul_Gage_20";
		m_tSoulRect.eSoulState = SOUL_20;
		break;

	case 10:
		m_tSoulRect.pFrameKey = L"Soul_Gage_10";
		m_tSoulRect.eSoulState = SOUL_10;
		break;

	case 0:
		m_tSoulRect.pFrameKey = L"Soul_Gage_0";
		m_tSoulRect.eSoulState = SOUL_0;
		break;
	}
}

void CSoul_Gage::Motion_Change()
{
	if (m_tSoulRect.eSoulState != m_tSoulRect.ePreSoulState)
	{
		switch (m_tSoulRect.eSoulState)
		{
		case SOUL_90:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 0;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 95;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_80:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 3;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_70:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 3;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_60:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 3;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_50:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 3;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_40:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 3;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_30:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 3;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_20:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 3;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_10:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 3;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;

		case SOUL_0:
			m_tSoulRect.tFrame.iFrameStart = 0;
			m_tSoulRect.tFrame.iFrameEnd = 0;
			m_tSoulRect.tFrame.iMotion = 0;
			m_tSoulRect.tFrame.dwFrameSpeed = 90;
			m_tSoulRect.tFrame.dwFrameTime = GetTickCount();
			break;
		}
		m_tSoulRect.ePreSoulState = m_tSoulRect.eSoulState;
	}
}

void CSoul_Gage::Move_Soul_Frame()
{
	if (m_tSoulRect.tFrame.dwFrameTime + m_tSoulRect.tFrame.dwFrameSpeed < GetTickCount())
	{
		m_tSoulRect.tFrame.iFrameStart++;

		m_tSoulRect.tFrame.dwFrameTime = GetTickCount();

		if (m_tSoulRect.tFrame.iFrameStart > m_tSoulRect.tFrame.iFrameEnd)
		{
			m_tSoulRect.tFrame.iFrameStart = 0;
		}
	}
}
