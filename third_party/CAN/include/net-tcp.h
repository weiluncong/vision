
// net-tcp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cusbcanfdx00udemoApp:
// �йش����ʵ�֣������ net-tcp.cpp
//

class CnettcpApp : public CWinAppEx
{
public:
	CnettcpApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CnettcpApp theApp;