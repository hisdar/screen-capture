#include "stdafx.h"
#include "tool/SCRectangleTool.h"
#include <GdiPlus.h>
#include <Windows.h>

SCRectangleTool::SCRectangleTool()
: m_color(0x00)
, m_size(0)
, m_rect(0, 0, 0, 0)
{
}

SCRectangleTool::~SCRectangleTool()
{
}

BOOL SCRectangleTool::Draw(CDC &cdc)
{
	Gdiplus::Graphics g(cdc.GetSafeHdc());
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::Color gColor(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
	Gdiplus::Pen gRectPen(gColor, (Gdiplus::REAL)m_size);

	g.DrawRectangle(&gRectPen, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height());

	return TRUE;
}

void SCRectangleTool::SetRect(CRect &rect)
{
	m_rect = rect;
}

void SCRectangleTool::SetRect(CPoint &startPoint, CPoint &endPoint)
{
	m_rect.SetRect(startPoint, endPoint);
}

void SCRectangleTool::SetColor(COLORREF color)
{
	m_color = color;
}

void SCRectangleTool::SetSize(int size)
{
	m_size = size;
}
