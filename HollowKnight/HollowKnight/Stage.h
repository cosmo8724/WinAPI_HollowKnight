#pragma once
#include "Scene.h"

class CStage : public CScene
{
private:


public:
	CStage();
	virtual ~CStage();

public:
	virtual void Initialize(void)	override;
	virtual int  Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

