#include "stdafx.h"
#include "Key_Mgr.h"

CKey_Mgr*	CKey_Mgr::m_pInstance = nullptr;

CKey_Mgr::CKey_Mgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}


CKey_Mgr::~CKey_Mgr()
{
}

bool CKey_Mgr::Key_Pressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}

bool CKey_Mgr::Key_Down(int _iKey)
{
	if (!m_bKeyState[_iKey] && GetAsyncKeyState(_iKey) & 0x8000)
	{
		m_bKeyState[_iKey] = true;
		return true;
	}
	return false;
}

bool CKey_Mgr::Key_Up(int _iKey)
{
	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{ 
		m_bKeyState[_iKey] = false;
		return true;
	}
	return false;
}

bool CKey_Mgr::Key_Double_Press(int _iKey)
{
	if (m_bKeyState[_iKey] && GetAsyncKeyState(_iKey) & 0x0001)
	{
		m_bKeyState[_iKey] = false;
		return true;
	}
	return false;
}

void CKey_Mgr::Key_Reset()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x80800))
			m_bKeyState[i] = false;

		else if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = true;
	}
}
