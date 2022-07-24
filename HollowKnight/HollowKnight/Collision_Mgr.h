#pragma once
#include "Obj.h"

class CCollision_Mgr
{
public:
	CCollision_Mgr();
	~CCollision_Mgr();

public:
	static void	Collision_Rect(list<CObj*> _Dest, list<CObj*> _Soul);
	static RECT	Collision_Ground(LONG &PlayerX, float &PlayerY);
	static void Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Soul);
	static bool Check_Rect(CObj* pDest, CObj* pSoul, float* pX, float* pY);
	static bool Check_Sphere(CObj* pDest, CObj* pSoul);
};