#include "stdafx.h"
#include "Mantis_Spike.h"
#include "Player.h"

#include "Scroll_Mgr.h"

CMantis_Spike::CMantis_Spike()
{
}


CMantis_Spike::~CMantis_Spike()
{
}

void CMantis_Spike::Initialize(void)
{
	m_tRect = { 1985, 520, 2290, 560 };
	Update_HitBox();
}

int CMantis_Spike::Update(void)
{
	return OBJ_NOEVENT;
}

void CMantis_Spike::Late_Update(void)
{
}

void CMantis_Spike::Render(HDC hDC)
{
	int iScrollX = (int)CScroll_Mgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	if (m_bVisibleHitBox)
	{
		HPEN myPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
		HPEN oldPen = (HPEN)::SelectObject(hDC, (HGDIOBJ)myPen);
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		Rectangle(hDC, m_tHitBox.left + iScrollX, m_tHitBox.top + iScrollY, m_tHitBox.right + iScrollX, m_tHitBox.bottom + iScrollY);

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
		SelectObject(hDC, oldPen);
		DeleteObject(myPen);
	}
}

void CMantis_Spike::Release(void)
{
}

void CMantis_Spike::Update_HitBox()
{
	m_tHitBox = m_tRect;
}

void CMantis_Spike::Collision_Event(CObj * _OtherObj, float _fColX, float _fColY)
{
}
