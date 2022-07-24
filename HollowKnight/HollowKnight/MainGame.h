#pragma once
#include "Include.h"

class CMainGame
{
private:
	HDC		m_hDC;
	DWORD	m_dwTime;
	TCHAR	m_szFPS[32];
	int		m_iFPS;

public:
	CMainGame();
	~CMainGame();

public:
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render();
	void	Release();
};

