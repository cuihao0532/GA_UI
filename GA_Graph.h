
// GA_Graph.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGA_GraphApp:
// �йش����ʵ�֣������ GA_Graph.cpp
//

class CGA_GraphApp : public CWinApp
{
public:
	CGA_GraphApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGA_GraphApp theApp;