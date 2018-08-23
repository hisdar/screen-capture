#include "stdafx.h"
#include "tool/text/SCTextTool.h"


SCTextTool::SCTextTool()
{
}


SCTextTool::~SCTextTool()
{
}

void SCTextTool::SetColor(COLORREF color)
{
	m_color = color;
}

void SCTextTool::SetSize(int size)
{
	m_size = size;
}

void SCTextTool::SetPoint(CPoint &point)
{
	m_point = point;
}

void SCTextTool::SetString(CString &string)
{
	m_string = string;
}

BOOL SCTextTool::Draw(CDC &cdc)
{
	CFont font;
	font.CreatePointFont(m_size * FONT_ADJUST, DEFAULT_FONT);

	CFont *oldFont = cdc.SelectObject(&font);
	COLORREF oldColor = cdc.SetTextColor(m_color);
	cdc.SetBkMode(TRANSPARENT);
	cdc.TextOut(m_point.x, m_point.y, m_string);

	// reset parameters
	cdc.SetTextColor(oldColor);
	cdc.SelectObject(oldFont);
	font.DeleteObject();

	return TRUE;
}
