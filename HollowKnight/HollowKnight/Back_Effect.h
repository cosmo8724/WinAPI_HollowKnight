#pragma once
#include "Obj.h"
class CBack_Effect : public CObj
{
public:
	CBack_Effect();
	virtual ~CBack_Effect();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;

private:
	void	Update_Info(float _fX, float _fY) { m_tInfo.fX = _fX; m_tInfo.fY = _fY; }
};