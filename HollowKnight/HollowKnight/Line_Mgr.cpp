#include "stdafx.h"
#include "Line_Mgr.h"

CLine_Mgr* CLine_Mgr::m_pInstance = nullptr;

CLine_Mgr::CLine_Mgr()
{
	ZeroMemory(m_tLinePoint, sizeof(LINEPOINT));
}


CLine_Mgr::~CLine_Mgr()
{
	Release();
}

bool CLine_Mgr::Collision_Line(float & fX, float * pY)
{
	if (m_LineList.empty())
		return false;

	CLine*		pTarget = nullptr;

	for (auto& iter : m_LineList)
	{
		if (fX >= iter->Get_LineInfo().tLeftPt.fX  &&
			fX <= iter->Get_LineInfo().tRightPt.fX)
		{
			pTarget = iter;
		}
	}

	if (!pTarget)
		return false;

	float x1 = pTarget->Get_LineInfo().tLeftPt.fX;
	float x2 = pTarget->Get_LineInfo().tRightPt.fX;
	float y1 = pTarget->Get_LineInfo().tLeftPt.fY;
	float y2 = pTarget->Get_LineInfo().tRightPt.fY;

	*pY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}

void CLine_Mgr::Save_Line()
{
	HANDLE	hFile = CreateFile(L"../Date/Line.dat",
								GENERIC_WRITE,
								NULL,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;

	for (auto& iter : m_LineList)
		WriteFile(hFile, &(iter->Get_LineInfo()), sizeof(LINE), &dwByte, nullptr);

	CloseHandle(hFile);
}

void CLine_Mgr::Load_Line()
{
	HANDLE	hFile = CreateFile(L"../Data/Line.dat",
								GENERIC_READ,
								NULL,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;
	LINE	tLine{};

	while (true)
	{
		ReadFile(hFile, &tLine, sizeof(LINE), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_LineList.push_back(new CLine(tLine));
	}

	CloseHandle(hFile);
}

void CLine_Mgr::Initialize()
{
	//Load_Line();

	LINEPOINT	tLinePt[2] = {
		{ 0.f, 590.f },
		{ 2369.f, 590.f }
	};
	m_LineList.push_back(new CLine(tLinePt[0], tLinePt[1]));
}

int CLine_Mgr::Update()
{
	return 0;
}

void CLine_Mgr::Late_Update()
{
}

void CLine_Mgr::Render(HDC hDC)
{
	for (auto& iter : m_LineList)
		iter->Render(hDC);
}

void CLine_Mgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), CDeleteObj());
	m_LineList.clear();
}
