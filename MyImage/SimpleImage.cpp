// SimpleImage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyImage.h"
#include "SimpleImage.h"
#include "MyImageDlg.h"


// CSimpleImage

IMPLEMENT_DYNAMIC(CSimpleImage, CWnd)

CSimpleImage::CSimpleImage(CWnd* pParent/*=NULL*/)
{
	int i;
	m_pParent = pParent;

	if (pParent)
		m_hParent = pParent->GetSafeHwnd();

	m_nIdxMk = 0;

	//m_pMil = M_NULL;
	m_pMil = new CLibMil(this);
	MIL_ID SystemId = m_pMil->GetSystemId();

	for (i = 0; i < MAX_DISP; i++)
	{
		m_pMilDispCad[i] = NULL;
		m_MilBufCad[i] = NULL;
		//m_pMilOvrCad[i] = NULL;
		//m_pMilDelOvrCad[i] = NULL;
		//m_pMilDispCad[i] = new CLibMilDisp(SystemId);

		m_pMilDispDef[i] = NULL;
		m_MilBufDef[i] = NULL;
		//m_pMilOvrDef[i] = NULL;
		//m_pMilDelOvrDef[i] = NULL;
		//m_pMilDispDef[i] = new CLibMilDisp(SystemId);
	}

	CreateWndForm(WS_CHILD | WS_OVERLAPPED);
	ThreadStart();
}

CSimpleImage::~CSimpleImage()
{
	ThreadStop();
	Sleep(30);
	t1.join();


	// for Def View.....
	for (int i = 0; i < MAX_DISP; i++)
	{
		// for CAD...
		//if (m_pMilOvrCad[i])
		//{
		//	delete m_pMilOvrCad[i];
		//	m_pMilOvrCad[i] = NULL;
		//}
		//if (m_pMilDelOvrCad[i])
		//{
		//	delete m_pMilDelOvrCad[i];
		//	m_pMilDelOvrCad[i] = NULL;
		//}
		if (m_MilBufCad[i])
		{
			MbufFree(m_MilBufCad[i]);
			m_MilBufCad[i] = NULL;
		}
		if (m_pMilDispCad[i])
		{
			delete m_pMilDispCad[i];
			m_pMilDispCad[i] = NULL;
		}

		// for Def....
		//if (m_pMilOvrDef[i])
		//{
		//	delete m_pMilOvrDef[i];
		//	m_pMilOvrDef[i] = NULL;
		//}
		//if (m_pMilDelOvrDef[i])
		//{
		//	delete m_pMilDelOvrDef[i];
		//	m_pMilDelOvrDef[i] = NULL;
		//}
		if (m_MilBufDef[i])
		{
			MbufFree(m_MilBufDef[i]);
			m_MilBufDef[i] = NULL;
		}
		if (m_pMilDispDef[i])
		{
			delete m_pMilDispDef[i];
			m_pMilDispDef[i] = NULL;
		}
	}

	if (m_pMil)
	{
		delete m_pMil;
		m_pMil = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSimpleImage, CWnd)
END_MESSAGE_MAP()



// CSimpleImage 메시지 처리기입니다.
BOOL CSimpleImage::CreateWndForm(DWORD dwStyle)
{
	if (!Create(NULL, _T("SimpleImage"), dwStyle, CRect(0, 0, 0, 0), m_pParent, (UINT)this))
	{
		AfxMessageBox(_T("CSimpleImage::Create() Failed!!!"));
		return FALSE;
	}

	return TRUE;
}


void CSimpleImage::ProcThrd(const LPVOID lpContext)
{
	CSimpleImage* pSimpleImage = reinterpret_cast<CSimpleImage*>(lpContext);

	while (pSimpleImage->ThreadIsAlive())
	{
		if (!pSimpleImage->ProcImage())
			break;
	}

	pSimpleImage->ThreadEnd();
}

BOOL CSimpleImage::ProcImage()
{
	Sleep(100);
	return TRUE;
}

void CSimpleImage::ThreadStart()
{
	m_bThreadStateEnd = FALSE;
	m_bThreadAlive = TRUE;
	t1 = std::thread(ProcThrd, this);
}

void CSimpleImage::ThreadStop()
{
	m_bThreadAlive = FALSE;
	MSG message;
	const DWORD dwTimeOut = 1000 * 60 * 3; // 3 Minute
	DWORD dwStartTick = GetTickCount();
	Sleep(30);
	while (!m_bThreadStateEnd)
	{
		if (GetTickCount() >= (dwStartTick + dwTimeOut))
		{
			AfxMessageBox(_T("WaitUntilThreadEnd() Time Out!!!", NULL, MB_OK | MB_ICONSTOP));
			return;
		}
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(30);
	}
}

void CSimpleImage::ThreadEnd()
{
	m_bThreadStateEnd = TRUE;
}

BOOL CSimpleImage::ThreadIsAlive()
{
	return m_bThreadAlive;
}


void CSimpleImage::DispMkInfo(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error."));
		return;
	}
		
	//ShowDispCad(nIdxMkInfo, nSerial, 0, m_nIdxDef[0]);
	//ShowOvrCad(nIdxMkInfo, nSerial);
	//nDefImg = pDoc->m_pPcr[0][nIdx]->m_pImg[m_nIdxDef[0]];
	//ShowDispDef(nIdxMkInfo, nSerial, 0, nDefImg);
	//ShowDefInfoUp(nIdxMkInfo);
	//WriteDefInfoUp(nSerial, nIdxMkInfo, m_nIdxDef[0], nDefImg);
	//SaveCadImgUp(nSerial, nIdxMkInfo, nDefImg);
	//m_nIdxMkInfo[0]++;
	//m_nIdxDef[0]++;
}

void CSimpleImage::ShiftInfo()
{
	for (int i = 0; i < MAX_DISP - 1; i++)
	{
		m_pMil->Copy(m_MilBufCad[i + 1], m_MilBufCad[i]);
		//m_pMil->Copy(m_MilOvrCad[i + 1], m_MilOvrCad[i]);
		m_pMil->Copy(m_MilBufDef[i + 1], m_MilBufDef[i]);
		//m_pMil->Copy(m_MilOvrDef[i + 1], m_MilOvrDef[i]);
	}
}

void CSimpleImage::DisplaySelect(int nKind, HWND hDispCtrl, CRect rtDispCtrl, int nIdx) // nKind : CAD_image[0], Defect_image[1]
{
	MIL_ID SystemId = m_pMil->GetSystemId();

	if (nKind == CAD_IMG)
	{
		SelDispCad(hDispCtrl, rtDispCtrl, nIdx);
		//MdispAlloc(SystemId, M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &m_MilBufCad[nIdx]);
		//m_pMilDispCad[nIdx]->DisplaySelect(m_MilBufCad[nIdx], hDispCtrl, rtDispCtrl);
	}

	else if (nKind == DEF_IMG)
	{
		SelDispDef(hDispCtrl, rtDispCtrl, nIdx);
		//MdispAlloc(SystemId, M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &m_MilBufDef[nIdx]);
		//m_pMilDispDef[nIdx]->DisplaySelect(m_MilBufDef[nIdx], hDispCtrl, rtDispCtrl);
	}
}

void CSimpleImage::SelDispCad(HWND hDispCtrl, CRect rtDispCtrl, int nIdx)
{
	MIL_ID SystemId = m_pMil->GetSystemId();

	// for CAD..........
	// Live Image Buffer set
	if (m_MilBufCad[nIdx])
	{
		MbufFree(m_MilBufCad[nIdx]);
		m_MilBufCad[nIdx] = NULL;
	}

	MbufAlloc2d(SystemId, DISP_SIZE_X, DISP_SIZE_Y, 1L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, &m_MilBufCad[nIdx]);
	MbufClear(m_MilBufCad[nIdx], M_NULL);

	// Mil Display set
	if (m_pMilDispCad[nIdx])
	{
		delete m_pMilDispCad[nIdx];
		m_pMilDispCad[nIdx] = NULL;
	}
	m_pMilDispCad[nIdx] = new CLibMilDisp(SystemId);
	m_pMilDispCad[nIdx]->DisplaySelect(m_MilBufCad[nIdx], hDispCtrl, rtDispCtrl);

	// Create Overlay
	m_pMilDispCad[nIdx]->CreateOverlay(M_COLOR_GREEN);
	Sleep(30);


}

void CSimpleImage::SelDispDef(HWND hDispCtrl, CRect rtDispCtrl, int nIdx)
{
	MIL_ID SystemId = m_pMil->GetSystemId();

	// for Defect..........
	// Live Image Buffer set
	if (m_MilBufDef[nIdx])
	{
		MbufFree(m_MilBufDef[nIdx]);
		m_MilBufDef[nIdx] = NULL;
	}

	MbufAlloc2d(SystemId, DISP_SIZE_X, DISP_SIZE_Y, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, &m_MilBufDef[nIdx]);
	MbufClear(m_MilBufDef[nIdx], M_NULL);

	// Mil Display set
	if (m_pMilDispDef[nIdx])
	{
		delete m_pMilDispDef[nIdx];
		m_pMilDispDef[nIdx] = NULL;
	}
	m_pMilDispDef[nIdx] = new CLibMilDisp(SystemId);
	m_pMilDispDef[nIdx]->DisplaySelect(m_MilBufDef[nIdx], hDispCtrl, rtDispCtrl);

	// Create Overlay
	m_pMilDispDef[nIdx]->CreateOverlay(M_COLOR_GREEN);
	Sleep(30);


}


void CSimpleImage::FreeDispCad(HWND hDispCtrl, CRect rtDispCtrl, int nIdx)
{
	// for CAD..........

	// Live Image Buffer set
	//if (m_pMilBufCad[nIdx])
	//{
	//	delete m_pMilBufCad[nIdx];
	//	m_pMilBufCad[nIdx] = NULL;
	//}

	// Mil Display set
	if (m_pMilDispCad[nIdx])
	{
		delete m_pMilDispCad[nIdx];
		m_pMilDispCad[nIdx] = NULL;
	}
}

void CSimpleImage::ShiftDisp()
{
	ShiftInfo();
	//for (int i = 0; i < MAX_DISP - 1; i++)
	//{
	//	MbufCopy(m_MilBufCad[i + 1], m_MilBufCad[i]);
	//	//MbufCopy(m_pMilOvrCad[i + 1]->m_MilBuffer, m_pMilOvrCad[i]->m_MilBuffer);
	//	//MbufCopy(m_pMilBufDef[i + 1]->m_MilImage, m_pMilBufDef[i]->m_MilImage);
	//}
}

void CSimpleImage::ShowDispCad(int nIdxMkInfo, int nSerial, int nDefPcs) // From 0 To 12...for Screen display.
{
	CString sPathOldFile = PATH_OLD_FILE;

	TCHAR cPath[MAX_PATH];
	if (m_MilBufCad[nIdxMkInfo] > 0)
	{
		CString sPath;
		sPath.Format(_T("%s%s\\%s\\%s\\CadImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefPcs);
		CFileFind findfile;
		if (findfile.FindFile(sPath))
		{
			_stprintf(cPath, _T("%s"), sPath);
			MbufLoad(cPath, m_MilBufCad[nIdxMkInfo]);
		}
	}
}

void CSimpleImage::ShowDispDef(int nIdxMkInfo, int nSerial, int nDefPcs) // From 0 To 12...for Screen display.
{
	CString sPathOldFile = PATH_OLD_FILE;

	TCHAR cPath[MAX_PATH];
	if (m_MilBufDef[nIdxMkInfo] > 0)
	{
		CString sPath;
		sPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefPcs);
		CFileFind findfile;
		if (findfile.FindFile(sPath))
		{
			_stprintf(cPath, _T("%s"), sPath);
			MbufLoad(cPath, m_MilBufDef[nIdxMkInfo]);
		}
	}
}

void CSimpleImage::SaveCadImg(int nIdxMkInfo, CString sPath)
{
	if (m_MilBufCad[nIdxMkInfo])
	{
		MbufSave(sPath, m_MilBufCad[nIdxMkInfo]);
	}
	else
	{
		AfxMessageBox(_T("SaveCadImg() Fail !!"));
	}
}

