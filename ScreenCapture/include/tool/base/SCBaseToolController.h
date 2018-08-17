#pragma once

#include "stdafx.h"

class SCBaseToolController
{
public:
	SCBaseToolController() {};
	virtual ~SCBaseToolController() {};

	virtual void OnLButtonDown(UINT nFlags, CPoint point) {};
	virtual void OnLButtonUp(UINT nFlags, CPoint point) {};
	virtual void OnMouseMove(UINT nFlags, CPoint point) {};
	virtual void OnRButtonDown(UINT nFlags, CPoint point) {};
	virtual void OnRButtonUp(UINT nFlags, CPoint point) {};
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point) {};
};

