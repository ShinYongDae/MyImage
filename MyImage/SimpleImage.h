#pragma once

#include <thread>

#include "LibMil.h"
#include "LibMilDisp.h"

#define MAX_DISP	12
#define DISP_SIZE_X	100
#define DISP_SIZE_Y	100
enum IMG_KIND { CAD_IMG = 0, DEF_IMG = 1 };

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
	CLibMil *m_pMil;
	CLibMilDisp *m_pMilDispCad[MAX_DISP], *m_pMilDispDef[MAX_DISP];
	MIL_ID m_MilBufCad[MAX_DISP], m_MilOvrCad[MAX_DISP];
	MIL_ID m_MilBufDef[MAX_DISP], m_MilOvrDef[MAX_DISP];

	void ShiftInfo();
	void DisplaySelect(int nKind, HWND hDispCtrl, CRect rtDispCtrl, int nIdx); // nKind : CAD_image[0], Defect_image[1]

public:
	CSimpleImage(CWnd* pParent = NULL);
	virtual ~CSimpleImage();
	static void ProcThrd(const LPVOID lpContext);

public:
	void DispMkInfo(int nSerial);
	void SelDispCad(HWND hDispCtrl, CRect rtDispCtrl, int nIdx);
	void SelDispDef(HWND hDispCtrl, CRect rtDispCtrl, int nIdx);
	void FreeDispCad(HWND hDispCtrl, CRect rtDispCtrl, int nIdx);
	void ShiftDisp();
	void ShowDispCad(int nIdxMkInfo, int nSerial, int nDefPcs);
	void ShowDispDef(int nIdxMkInfo, int nSerial, int nDefPcs);
	void SaveCadImg(int nIdxMkInfo, CString sPath);


protected:
	void ThreadEnd();
	BOOL ProcImage();
	BOOL ThreadIsAlive();


protected:
	DECLARE_MESSAGE_MAP()
};


