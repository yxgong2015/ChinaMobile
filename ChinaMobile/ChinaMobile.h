
// ChinaMobile.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChinaMobileApp:
// �йش����ʵ�֣������ ChinaMobile.cpp
//

class CChinaMobileApp : public CWinApp
{
public:
	CChinaMobileApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChinaMobileApp theApp;