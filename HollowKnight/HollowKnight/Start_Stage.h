#pragma once
#include "Scene.h"
class CStart_Stage : public CScene
{
private:
	RECT	m_tPortal;
public:
	CStart_Stage();
	virtual ~CStart_Stage();

public:
	virtual void Initialize(void)	override;
	virtual int  Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

