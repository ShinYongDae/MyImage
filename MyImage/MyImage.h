
// MyImage.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMyImageApp:
// �� Ŭ������ ������ ���ؼ��� MyImage.cpp�� �����Ͻʽÿ�.
//

class CMyImageApp : public CWinApp
{
public:
	CMyImageApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMyImageApp theApp;