#pragma once
#include "Include.h"

class CLine
{
private:
	LINE	m_tLine;

public:
	CLine();
	CLine(LINEPOINT& tLeftPt, LINEPOINT& tRightPt);
	CLine(LINE& rLine);
	~CLine();

public:
	const LINE& Get_LineInfo() const { return m_tLine; }
	void		Render(HDC hDC);
};

