#include "StdAfx.h"
#include "UserDraw.h"


CUserDraw::CUserDraw(void)
	: m_startPoint(0)
	, m_endPoint(0)
	, m_shape(0)
	, m_color(RGB(0,0,0))
	, m_size(2)
{
}

CUserDraw::CUserDraw(const CUserDraw &userDraw) {
	CopyUserDraw(userDraw);
}

void CUserDraw::CopyUserDraw(const CUserDraw &userDraw) {
	m_startPoint = userDraw.m_startPoint;
	m_endPoint = userDraw.m_endPoint;
	m_shape = userDraw.m_shape;
	m_str = userDraw.m_str;
	m_color = userDraw.m_color;
	m_size = userDraw.m_size;
}

CUserDraw & CUserDraw::operator=(const CUserDraw &userDraw) {
	CopyUserDraw(userDraw);
	return *this;
}


CUserDraw::~CUserDraw(void)
{
}
