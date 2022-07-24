#pragma once

#include "Include.h"

class CScene
{
protected:
	float	m_fPlayerX = 0.f;

public:
	CScene();
	virtual ~CScene();

public:
	virtual void	Initialize(void)	PURE;
	virtual int		Update(void)		PURE;
	virtual void	Late_Update(void)	PURE;
	virtual void	Render(HDC hDC)		PURE;
	virtual void	Release(void)		PURE;
};