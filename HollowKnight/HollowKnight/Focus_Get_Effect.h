#pragma once
#include "Obj.h"
class CFocus_Get_Effect : public CObj
{
public:
	CFocus_Get_Effect();
	virtual ~CFocus_Get_Effect();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

