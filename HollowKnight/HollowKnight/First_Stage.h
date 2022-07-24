#pragma once
#include "Scene.h"
class CFirst_Stage : public CScene
{
public:
	CFirst_Stage();
	virtual ~CFirst_Stage();

public:
	virtual void Initialize(void)	override;
	virtual int  Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

