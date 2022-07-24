#pragma once
#include "Include.h"

class CKey_Mgr
{
private:
	static	CKey_Mgr*	m_pInstance;
	bool				m_bKeyState[VK_MAX];

private:
	CKey_Mgr();
	~CKey_Mgr();

public:
	bool	Key_Pressing(int _iKey);
	bool	Key_Down(int _iKey);
	bool	Key_Up(int _iKey);
	bool	Key_Double_Press(int _iKey);
	void	Key_Reset();

public:
	static CKey_Mgr*	Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CKey_Mgr;

		return m_pInstance;
	}
	static void			Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

