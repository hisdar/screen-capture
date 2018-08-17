#pragma once

#include "tool/base/SCBaseToolView.h"
#include "base/SCObject.h"

/// @brief �ɰ湤�ߣ����������ɰ�
class SCMaskTool : public SCBaseToolView, public SCObject
{
public:
	SCMaskTool();
	virtual ~SCMaskTool();

	virtual BOOL Draw(CDC &cdc);

	void SetRect(CRect &rect);
	void SetRect(int left, int top, int right, int bottom);

private:
	BOOL DrawMetteRect(CDC *pCDC, int startX, int startY, int width, int height);

private:
	CRect m_rect;
};

