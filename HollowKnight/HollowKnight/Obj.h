#pragma once
#include "Include.h"

class CObj
{
protected:
	// struct
	STAT		m_tStat;
	INFO		m_tInfo;
	RECT		m_tRect;
	RECT		m_tHitBox;
	FRAME		m_tFrame;

	// enum
	DIRID		m_eDir;
	OBJECT_STATE	m_eState = GROUND;
	OBJECT_LOOK m_eLook = LOOK_END;
	
	// value
	float		m_fSpeed	= 0.f;
	float		m_fAngle	= 0.f;
	float		m_fDiagonal = 0.f;

	bool		m_bDead = false;
	bool		m_bDamaged = false;
	DWORD		m_dwDamagedTime;
	CObj*		m_pTarget;
	TCHAR*		m_pFrameKey;

public:
	CObj();
	virtual ~CObj();

public:
	virtual void	Initialize(void)	PURE;
	virtual int		Update(void)		PURE;
	virtual void	Late_Update(void)	PURE;
	virtual void	Render(HDC hDC)		PURE;
	virtual void	Release(void)		PURE;
	virtual void    Collision_Event(CObj* _OtherObj, float _fColX, float _fColY) {}

public:
	void	Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	void	Set_MinusHp(int _iDmg) { m_tStat.iHp -= _iDmg; }
	void	Set_PosX(float _fX) { m_tInfo.fX += _fX; }
	void	Set_PosY(float _fY) { m_tInfo.fY += _fY; }
	void	Set_Dir(DIRID eDir)	{ m_eDir = eDir; }
	void	Set_Look(OBJECT_LOOK _eLook) { m_eLook = _eLook; }
	void	Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void	Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void	Set_FrameKey(TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
	void	Set_FrameMotion(OBJECT_LOOK _eLook) { m_tFrame.iMotion = _eLook; }
	void	Set_State(OBJECT_STATE _State) { m_eState = _State; }

	const STAT&		Get_Stat()		const { return m_tStat; }
	const RECT&		Get_Rect()		const { return m_tRect; }
	const RECT&		Get_HitBox()	const { return m_tHitBox; }
	const INFO&		Get_Info()		const { return m_tInfo; }
	const bool		Get_Dead()		const { return m_bDead; }
	const OBJECT_LOOK&	Get_Look()	const { return m_eLook; }

protected:
	virtual void	Update_Rect();
	virtual bool	Move_Frame(int _InitFrame = 0);
	virtual void	Update_HitBox() {}
};

