#pragma once

// Window Size
#define		WINCX			1600
#define		WINCY			1000

// Back Ground Size
#define		START_STAGE_CX		2881
#define		START_STAGE_CY		1280
#define		FIRST_STAGE_CX		4407
#define		FIRST_STAGE_CY		1280
#define		MANTIS_LORDS_STAGE_CX	4819
#define		MANTIS_LORDS_STAGE_CY	8434
#define		THE_RADIANCE_STAGE_CX	3000
#define		THE_RADIANCE_STAGE_CY	5000

// Value
#define		PURE			= 0
#define		PI				3.141592f
#define		VK_MAX			0xff
#define		GRAVITY			1.1f
#define		EPSILON			0.0001f

// Event
#define		OBJ_NOEVENT		0
#define		OBJ_DEAD		1

// Extern
extern		HWND			g_hWnd;
extern		bool			m_bVisibleHitBox;
extern		int				g_iFrameSpeed;