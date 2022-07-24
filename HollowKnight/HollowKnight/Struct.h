#pragma once

typedef struct tagStat
{
	int		iMaxHp;
	int		iHp;
	int		iMaxSoul;
	int		iSoul;
	int		iDamage;
}STAT;

typedef struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;
}INFO;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;

	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;
}FRAME;

typedef struct tagHPRect
{
	LONG		left;
	LONG		top;
	LONG		right;
	LONG		bottom;
	HP_STATE	eState;
	HP_STATE	ePreState;
	SOUL_STATE	eSoulState;
	SOUL_STATE	ePreSoulState;
	TCHAR*		pFrameKey;
	int			iLoopFrameStart = 0;
	tagFrame	tFrame;
}HPRECT;

typedef struct tagPoint
{
	float	fX;
	float	fY;

	tagPoint() { ZeroMemory(this, sizeof(tagPoint)); }
	tagPoint(float _fX, float _fY) : fX(_fX), fY(_fY) {}
}LINEPOINT;

typedef struct tagLine
{
	LINEPOINT	tLeftPt;
	LINEPOINT	tRightPt;

	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(LINEPOINT& _tLeftPt, LINEPOINT& _tRightPt)
		: tLeftPt(_tLeftPt), tRightPt(_tRightPt)
	{
	}
}LINE;