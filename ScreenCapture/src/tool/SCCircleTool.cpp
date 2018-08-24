#include "stdafx.h"
#include "tool/SCCircleTool.h"
#include <GdiPlus.h>
#include <Windows.h>

SCCircleTool::SCCircleTool()
: m_color(0x00)
, m_size(0)
, m_rect(0, 0, 0, 0)
{
}


SCCircleTool::~SCCircleTool()
{
}

BOOL SCCircleTool::Draw(CDC &cdc)
{
	Gdiplus::Graphics g(cdc.GetSafeHdc());
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::Color gColor(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
	Gdiplus::Pen gCirclePen(gColor, (Gdiplus::REAL)m_size);
	
	g.DrawEllipse(&gCirclePen, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height());

	return TRUE;
}

void SCCircleTool::SetRect(CRect &rect)
{
	m_rect = rect;
}

void SCCircleTool::SetRect(CPoint &startPoint, CPoint &endPoint)
{
	m_rect.SetRect(startPoint, endPoint);
}

void SCCircleTool::SetColor(COLORREF color)
{
	m_color = color;
}

void SCCircleTool::SetSize(int size)
{
	m_size = size;
}
