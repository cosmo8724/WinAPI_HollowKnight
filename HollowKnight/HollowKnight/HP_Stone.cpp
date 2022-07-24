#include "stdafx.h"
#include "HP_Stone.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"

CHP_Stone::CHP_Stone()
{
}


CHP_Stone::~CHP_Stone()
{
}

void CHP_Stone::Initialize(void)
{
	m_tInfo = { 240.f, 140.f, 60.f, 124.f };

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/HP_Stone.bmp", L"Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/HP_Stone_Break.bmp", L"Break");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/UI/HP_Stone_Refill.bmp", L"Refill");
	
	for (int i = 0; i < 5; ++i)
	{
		m_tRectArray[i].pFrameKey = L"Idle";

		m_tRectArray[i].tFrame.iFrameStart = 0;
		m_tRectArray[i].tFrame.iFrameEnd = 44;
		m_tRectArray[i].tFrame.iMotion = 0;
		m_tRectArray[i].tFrame.dwFrameSpeed = 40;
		m_tRectArray[i].tFrame.dwFrameTime = GetTickCount();
	}

	m_iPlayerCurHP = CObj_Mgr::Get_Instance()->Get_Player()->Get_Stat().iHp;
	m_iPlayerLastHP = m_iPlayerCurHP;
}

int CHP_Stone::Update(void)
{
	Update_State();
	Motion_Change();
	Move_Array_Frame();
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CHP_Stone::Late_Update(void)
{
}

void CHP_Stone::Render(HDC hDC)
{
	HDC hMemDC[5];

	int iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	for (int i = 0; i < 5; ++i)
	{
		hMemDC[i] = CBmp_Mgr::Get_Instance()->Find_Img(m_tRectArray[i].pFrameKey);

		GdiTransparentBlt(hDC,
			m_tRectArray[i].left,
			m_tRectArray[i].top,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC[i],
			(int)m_tInfo.fCX * m_tRectArray[i].tFrame.iFrameStart,
			(int)m_tInfo.fCY * m_tRectArray[i].tFrame.iMotion,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			RGB(11, 11, 11));
	}

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

void CHP_Stone::Release(void)
{
}

void CHP_Stone::Motion_Change()
{
	for (int i = 0; i < 5; ++i)
	{
		if (m_tRectArray[i].eState != m_tRectArray[i].ePreState)
		{
			switch (m_tRectArray[i].eState)
			{
			case HP_IDLE:
				m_tRectArray[i].tFrame.iFrameStart = 0;
				m_tRectArray[i].tFrame.iFrameEnd = 44;
				m_tRectArray[i].tFrame.iMotion = 0;
				m_tRectArray[i].tFrame.dwFrameSpeed = 40;
				m_tRectArray[i].tFrame.dwFrameTime = GetTickCount();
				m_tRectArray[i].iLoopFrameStart = 0;
				break;

			case HP_FILL:
				m_tRectArray[i].tFrame.iFrameStart = 0;
				m_tRectArray[i].tFrame.iFrameEnd = 5;
				m_tRectArray[i].tFrame.iMotion = 0;
				m_tRectArray[i].tFrame.dwFrameSpeed = 40;
				m_tRectArray[i].tFrame.dwFrameTime = GetTickCount();
				m_tRectArray[i].iLoopFrameStart = 5;
				break;

			case HP_BREAK:
				m_tRectArray[i].tFrame.iFrameStart = 0;
				m_tRectArray[i].tFrame.iFrameEnd = 5;
				m_tRectArray[i].tFrame.iMotion = 0;
				m_tRectArray[i].tFrame.dwFrameSpeed = 40;
				m_tRectArray[i].tFrame.dwFrameTime = GetTickCount();
				m_tRectArray[i].iLoopFrameStart = 5;
				break;
			}
			m_tRectArray[i].ePreState = m_tRectArray[i].eState;
		}
	}
}

void CHP_Stone::Move_Array_Frame()
{
	for (int i = 0; i < 5; ++i)
	{
		if (m_tRectArray[i].tFrame.dwFrameTime + m_tRectArray[i].tFrame.dwFrameSpeed < GetTickCount())
		{
			m_tRectArray[i].tFrame.iFrameStart++;

			m_tRectArray[i].tFrame.dwFrameTime = GetTickCount();

			if (m_tRectArray[i].tFrame.iFrameStart > m_tRectArray[i].tFrame.iFrameEnd)
			{
				m_tRectArray[i].tFrame.iFrameStart = m_tRectArray[i].iLoopFrameStart;
			}
		}
	}
}

void CHP_Stone::Update_Rect()
{
	for (int i = 0; i < 5; ++i)
	{
		m_tRectArray[i].left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f)) + i * m_tInfo.fCX;
		m_tRectArray[i].top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
		m_tRectArray[i].right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f)) + i * m_tInfo.fCX;
		m_tRectArray[i].bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
	}
}

void CHP_Stone::Update_HitBox()
{
	m_tHitBox.left = m_tRectArray[0].left;
	m_tHitBox.top = m_tRectArray[0].top;
	m_tHitBox.right = m_tRectArray[4].right;
	m_tHitBox.bottom = m_tRectArray[0].bottom;
}

void CHP_Stone::Update_State()
{
	m_iPlayerCurHP = CObj_Mgr::Get_Instance()->Get_Player()->Get_Stat().iHp;
	
	if (m_iPlayerCurHP < m_iPlayerLastHP)		// Reduce HP
	{
		switch (m_iPlayerCurHP)
		{
		case 40:
			m_tRectArray[4].pFrameKey = L"Break";
			m_tRectArray[4].eState = HP_BREAK;
			break;

		case 30:
			m_tRectArray[3].pFrameKey = L"Break";
			m_tRectArray[3].eState = HP_BREAK;
			break;

		case 20:
			m_tRectArray[2].pFrameKey = L"Break";
			m_tRectArray[2].eState = HP_BREAK;
			break;

		case 10:
			m_tRectArray[1].pFrameKey = L"Break";
			m_tRectArray[1].eState = HP_BREAK;
			break;
			
		case 0:
			m_tRectArray[0].pFrameKey = L"Break";
			m_tRectArray[0].eState = HP_BREAK;
			break;
		}
	}
	else if (m_iPlayerCurHP > m_iPlayerLastHP)	// Increase HP
	{
		switch (m_iPlayerCurHP)
		{
		case 50:
			m_tRectArray[4].pFrameKey = L"Refill";
			m_tRectArray[4].eState = HP_FILL;
			break;

		case 40:
			m_tRectArray[3].pFrameKey = L"Refill";
			m_tRectArray[3].eState = HP_FILL;
			break;

		case 30:
			m_tRectArray[2].pFrameKey = L"Refill";
			m_tRectArray[2].eState = HP_FILL;
			break;

		case 20:
			m_tRectArray[1].pFrameKey = L"Refill";
			m_tRectArray[1].eState = HP_FILL;
			break;

		case 10:
			m_tRectArray[0].pFrameKey = L"Refill";
			m_tRectArray[0].eState = HP_FILL;
			break;
		}
	}

	m_iPlayerLastHP = m_iPlayerCurHP;

	for (int i = 0; i < 5; ++i)
	{
		if (m_tRectArray[i].ePreState == HP_FILL && m_tRectArray[i].tFrame.iFrameStart == m_tRectArray[i].tFrame.iFrameEnd)
		{
			m_tRectArray[i].pFrameKey = L"Idle";
			m_tRectArray[i].eState = HP_IDLE;
		}
	}
}
