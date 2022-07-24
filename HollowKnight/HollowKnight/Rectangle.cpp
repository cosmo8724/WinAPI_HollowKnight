#include "stdafx.h"
#include "Rectangle.h"
#include "Player.h"

#include "Scroll_Mgr.h"

CRectangle::CRectangle()
{
}

CRectangle::CRectangle(INFO _tInfo)
{
	m_tInfo = _tInfo;
}

CRectangle::~CRectangle()
{
}

void CRectangle::Initialize(void)
{
}

int CRectangle::Update(void)
{
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CRectangle::Late_Update(void)
{
}

void CRectangle::Render(HDC hDC)
{
	int		iScrollX = (int)CScroll_Mgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	if (m_bVisibleHitBox)
	{
		HPEN myPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		HPEN oldPen = (HPEN)::SelectObject(hDC, (HGDIOBJ)myPen);
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
		SelectObject(hDC, oldPen);
		DeleteObject(myPen);
	}
}

void CRectangle::Release(void)
{
}

void CRectangle::Update_HitBox()
{
	m_tHitBox = m_tRect;
}

void CRectangle::Collision_Event(CObj* _OtherObj, float _fColX, float _fColY)
{
	CPlayer*	pPlayer = dynamic_cast<CPlayer*>(_OtherObj);

	if (pPlayer)
	{
		POINT	pPlayerPoint{ (pPlayer->Get_HitBox().left + pPlayer->Get_HitBox().right) * 0.5, (pPlayer->Get_HitBox().top + pPlayer->Get_HitBox().bottom) * 0.5f };

		if (_fColX < _fColY)
		{
			if (pPlayerPoint.y > m_tRect.top + 30 && pPlayerPoint.y < m_tRect.bottom - 30)
			{
				pPlayer->Set_State(WALL);
				pPlayer->Set_DoubleJumpCnt(1);
			}
			else
			{
				pPlayer->Set_State(AIR);
				pPlayer->Set_IsJump(true);
				//pPlayer->Set_CurJumpSpeed(0.f);
			}
			if (pPlayerPoint.x > m_tInfo.fX)	// 우충돌
				pPlayer->Set_PosX(_fColX * 1.1f);

			else
				pPlayer->Set_PosX(-_fColX * 1.1f);
		}
		else
		{
			if (pPlayerPoint.y > m_tInfo.fY)	// 하충돌
			{
				pPlayer->Set_IsJump(true);
				pPlayer->Set_CurJumpSpeed(0.f);
				pPlayer->Set_PosY(_fColY * 1.1f);
			}
			else
			{
				pPlayer->Set_State(GROUND);
				pPlayer->Set_PosY(_fColY * -1.1f);
			}
		}
	}
}