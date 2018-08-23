#include "stdafx.h"
#include "tool/arrow/SCArrowTool.h"
#include <GdiPlus.h>
#include <Windows.h>

SCArrowTool::SCArrowTool()
{
}


SCArrowTool::~SCArrowTool()
{
}


BOOL SCArrowTool::Draw(CDC &cdc)
{
	Gdiplus::Graphics g(cdc.GetSafeHdc());
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::Color gColor(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
	Gdiplus::Pen gArrowPen(gColor, (Gdiplus::REAL)m_size);

	CPoint startPoint(m_rect.left, m_rect.top);
	CPoint endPoint(m_rect.right, m_rect.bottom);

	g.DrawLine(&gArrowPen, startPoint.x, startPoint.y, endPoint.x, endPoint.y);

	// draw arrow
	int x1 = endPoint.x - 10;
	if (endPoint.x < startPoint.x) {
		x1 = startPoint.x + 10;
	}

	int y1 = endPoint.y - 10;
	if (endPoint.y < startPoint.y) {
		y1 = endPoint.y + 10;
	}

	return TRUE;
}

void SCArrowTool::SetRect(CRect &rect)
{
	m_rect = rect;
}

void SCArrowTool::SetRect(CPoint &startPoint, CPoint &endPoint)
{
	m_rect.SetRect(startPoint, endPoint);
}

void SCArrowTool::SetColor(COLORREF color)
{
	m_color = color;
}

void SCArrowTool::SetSize(int size)
{
	m_size = size;
}