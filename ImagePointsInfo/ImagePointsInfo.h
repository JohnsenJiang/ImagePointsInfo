
// ImagePointsInfo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImagePointsInfoApp:
// �йش����ʵ�֣������ ImagePointsInfo.cpp
//

class CImagePointsInfoApp : public CWinApp
{
public:
	CImagePointsInfoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CImagePointsInfoApp theApp;