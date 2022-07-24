#pragma once
#include "Scene.h"
class CThe_Radiance_Stage : public CScene
{
private:
	RECT	m_tRect1;
	RECT	m_tRect2;
	RECT	m_tRect3;

public:
	CThe_Radiance_Stage();
	virtual ~CThe_Radiance_Stage();

public:
	virtual void Initialize(void)	override;
	virtual int  Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

