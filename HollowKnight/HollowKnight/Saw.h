#pragma once
#include "Obj.h"
class CSaw : public CObj
{
public:
	CSaw();
	virtual ~CSaw();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
};

