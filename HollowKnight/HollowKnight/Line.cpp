#include "stdafx.h"
#include "Line.h"


CLine::CLine()
{
	ZeroMemory(&m_tLine, sizeof(LINE));
}

CLine::CLine(LINEPOINT & tLeftPt, LINEPOINT & tRightPt)
	: m_tLine(tLeftPt, tRightPt)
{
}

CLine::CLine(LINE & rLine)
	: m_tLine(rLine)
{
}


CLine::~CLine()
{
}

void CLine::Render(HDC hDC)
{
}
