#include "stdafx.h"
#include "Collision_Mgr.h"
#include "Obj_Mgr.h"

#include "Rectangle.h"

CCollision_Mgr::CCollision_Mgr()
{
}


CCollision_Mgr::~CCollision_Mgr()
{
}

void CCollision_Mgr::Collision_Rect(list<CObj*> _Dest, list<CObj*> _Soul)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Soul : _Soul)
		{
			float		fX = 0.f, fY = 0.f;

			if (Check_Rect(Dest, Soul, &fX, &fY))
			{
				Dest->Collision_Event(Soul, fX, fY);
				Soul->Collision_Event(Dest, fX, fY);
			}
		}
	}
}

void CCollision_Mgr::Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Soul)
{
}

RECT CCollision_Mgr::Collision_Ground(LONG &PlayerX, float &PlayerY)
{
	CObj*			CurrentRect = nullptr;
	list<CObj*>*	RectList = CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_RECTANGLE);

	for (auto& Rect : *RectList)
	{
		if (PlayerX >= Rect->Get_Rect().left && PlayerX <= Rect->Get_Rect().right && PlayerY - Rect->Get_Rect().top < 50)
		{
			CurrentRect = Rect;
		}
	}
	if (CurrentRect == nullptr)
		return RECT();
	else
		return CurrentRect->Get_Rect();
}

bool CCollision_Mgr::Check_Rect(CObj * pDest, CObj * pSoul, float * pX, float * pY)
{
	POINT		tDestPoint{ (pDest->Get_HitBox().left + pDest->Get_HitBox().right) / 2, (pDest->Get_HitBox().top + pDest->Get_HitBox().bottom) / 2 };
	POINT		tSoulPoint{ (pSoul->Get_HitBox().left + pSoul->Get_HitBox().right) / 2, (pSoul->Get_HitBox().top + pSoul->Get_HitBox().bottom) / 2 };

	LONG		fDestRadiusX = pDest->Get_HitBox().right - pDest->Get_HitBox().left;
	LONG		fDestRadiusY = pDest->Get_HitBox().bottom - pDest->Get_HitBox().top;
	LONG		fSoulRadisuX = pSoul->Get_HitBox().right - pSoul->Get_HitBox().left;
	LONG		fSoulRadisuY = pSoul->Get_HitBox().bottom - pSoul->Get_HitBox().top;

	//float		fWidth = abs(pDest->Get_Info().fX - pSoul->Get_Info().fX);
	//float		fHeight = abs(pDest->Get_Info().fY - pSoul->Get_Info().fY);

	LONG		fWidth = abs(tDestPoint.x - tSoulPoint.x);
	LONG		fHeight = abs(tDestPoint.y - tSoulPoint.y);

	float		fRadiusX = (fDestRadiusX + fSoulRadisuX) * 0.5f;
	float		fRadiusY = (fDestRadiusY + fSoulRadisuY) * 0.5f;

	if ((fWidth < fRadiusX) && (fHeight < fRadiusY))
	{
		*pX = fRadiusX - fWidth;
		*pY = fRadiusY - fHeight;

		return true;
	}
	return false;
}

bool CCollision_Mgr::Check_Sphere(CObj * pDest, CObj * pSoul)
{
	return false;
}
