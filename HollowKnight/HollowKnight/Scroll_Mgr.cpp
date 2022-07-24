#include "stdafx.h"
#include "Scroll_Mgr.h"
#include "Obj_Mgr.h"

CScroll_Mgr*	CScroll_Mgr::m_pInstance = nullptr;

CScroll_Mgr::CScroll_Mgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScroll_Mgr::~CScroll_Mgr()
{
}

void CScroll_Mgr::Initialize()
{
	m_eCurScene = CScene_Mgr::Get_Instance()->Get_SceneID();

	switch (m_eCurScene)
	{
	case SC_START_STAGE:
		m_fScrollX = 0.f;
		m_fScrollY = -320.f;
		break;

	case SC_FIRST_STAGE:
		m_fScrollX = 0.f;
		m_fScrollY = -320.f;
		break;

	case SC_MANTIS_LORDS_STAGE:
		m_fScrollX = 0.f;
		m_fScrollY = -6800.f;
		break;
	}
}

void CScroll_Mgr::Scroll_Lock()
{
	m_eCurScene = CScene_Mgr::Get_Instance()->Get_SceneID();

	switch (m_eCurScene)
	{
	case SC_START_STAGE:
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;

		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;
		// Need Modify
		if (WINCX - START_STAGE_CX > m_fScrollX)
			m_fScrollX = WINCX - START_STAGE_CX;

		if (WINCY - START_STAGE_CY > m_fScrollY)
			m_fScrollY = WINCY - START_STAGE_CY;
		break;

	case SC_FIRST_STAGE:
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;

		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;
		// Need Modify
		if (WINCX - FIRST_STAGE_CX > m_fScrollX)
			m_fScrollX = WINCX - FIRST_STAGE_CX;

		if (WINCY - FIRST_STAGE_CY > m_fScrollY)
			m_fScrollY = WINCY - FIRST_STAGE_CY;
		break;

	case SC_MANTIS_LORDS_STAGE:
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;

		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCX - MANTIS_LORDS_STAGE_CX > m_fScrollX)
			m_fScrollX = WINCX - MANTIS_LORDS_STAGE_CX;

		if (WINCY - MANTIS_LORDS_STAGE_CY > m_fScrollY)
			m_fScrollY = WINCY - MANTIS_LORDS_STAGE_CY;
		break;

	case SC_THE_RADIANCE_STAGE:
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;

		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCX - THE_RADIANCE_STAGE_CX > m_fScrollX)
			m_fScrollX = WINCX - THE_RADIANCE_STAGE_CX;

		if (WINCY - THE_RADIANCE_STAGE_CY > m_fScrollY)
			m_fScrollY = WINCY - THE_RADIANCE_STAGE_CY;
		break;
	}
}
