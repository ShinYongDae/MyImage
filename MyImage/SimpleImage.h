#pragma once

#include <thread>
#include "LibMil.h"

#define MAX_DISP	12

// CSimpleImage

class CSimpleImage : public CWnd
{
	DECLARE_DYNAMIC(CSimpleImage)

	CWnd* m_pParent;
	HWND m_hParent;

	CString m_sPath;
	BOOL CreateWndForm(DWORD dwStyle);

	BOOL m_bThreadAlive, m_bThreadStateEnd;
	std::thread t1;
	void ThreadStart();
	void ThreadStop();

	int m_nIdxMk;
	CLibMil m_Mil;
	MIL_ID m_MilDispCad[MAX_DISP], m_MilDispDef[MAX_DISP];
	MIL_ID m_MilBufCad[MAX_DISP], m_MilOvrCad[MAX_DISP], m_MilBufDef[MAX_DISP];

	void ShiftInfo();

public:
	CSimpleImage(CWnd* pParent = NULL);
	virtual ~CSimpleImage();
	static void ProcThrd(const LPVOID lpContext);

public:
	void DispMkInfo(int nSerial);


protected:
	void ThreadEnd();
	BOOL ProcImage();
	BOOL ThreadIsAlive();


protected:
	DECLARE_MESSAGE_MAP()
};


