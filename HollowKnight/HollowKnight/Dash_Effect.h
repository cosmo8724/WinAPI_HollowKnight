#pragma once
#include "Obj.h"
class CDash_Effect : public CObj
{
private:

public:
	CDash_Effect();
	virtual ~CDash_Effect();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

