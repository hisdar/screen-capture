#pragma once
class SCBaseToolView
{
public:
	SCBaseToolView() {};
	virtual ~SCBaseToolView() {};

	virtual BOOL Draw(CDC &cdc) = 0;
};

