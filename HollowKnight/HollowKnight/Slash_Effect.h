#pragma once
#include "Obj.h"
class CSlash_Effect : public CObj
{
private:
	bool	m_bHit = false;

public:
	CSlash_Effect();
	virtual ~CSlash_Effect();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
	virtual void Collision_Event(CObj* _OtherObj, float _fColX, float _fColY) override;

private:
	void	Update_Info(float _fX, float _fY) { m_tInfo.fX = _fX; m_tInfo.fY = _fY; }
};

