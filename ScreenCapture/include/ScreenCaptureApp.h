
// ScreenCaptureApp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#include "stdafx.h"

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CScreenCaptureApp:
// �йش����ʵ�֣������ ScreenCapture.cpp
//

class CScreenCaptureApp : public CWinApp
{
public:
	CScreenCaptureApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CScreenCaptureApp theApp;