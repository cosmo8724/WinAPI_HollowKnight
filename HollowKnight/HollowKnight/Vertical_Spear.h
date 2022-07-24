#pragma once
#include "Obj.h"
class CVertical_Spear : public CObj
{
private:
	int		m_iLoopFrameStart = 7;
	bool	m_bSetPosition = false;
	bool	m_bSoundChange = false;

public:
	CVertical_Spear();
	virtual ~CVertical_Spear();

public:
	void	Set_Motion(OBJECT_LOOK _eLook) { m_tFrame.iMotion = _eLook; }

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
	virtual void Collision_Event(CObj* _OtherObj, float _fColX, float _fColY) override;
};

