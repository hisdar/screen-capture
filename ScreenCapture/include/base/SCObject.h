#pragma once
class SCObject
{
public:
	SCObject();
	virtual ~SCObject();

	BOOL CreateCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp);
	BOOL CopyCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp);
	BOOL DCToImage(CDC &dc, CImage &image);

	static BOOL SaveBmp(CDC &dc, LPCTSTR pszFileName);	
};

