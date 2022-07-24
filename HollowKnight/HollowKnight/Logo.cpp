#include "stdafx.h"
#include "Logo.h"

#include "Bmp_Mgr.h"
#include "Key_Mgr.h"
#include "Scene_Mgr.h"

CLogo::CLogo()
{
}


CLogo::~CLogo()
{
}

void CLogo::Initialize()
{
	//CBmp_Mgr::Get_Instance()->Insert_Bmp("", "");
}

int CLogo::Update()
{
	return 0;
}

void CLogo::Late_Update()
{
	if (CKey_Mgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CScene_Mgr::Get_Instance()->Scene_Change(SC_MENU);
		return;
	}
}

void CLogo::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"Logo");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
}
