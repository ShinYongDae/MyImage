// SimpleImage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyImage.h"
#include "SimpleImage.h"


// CSimpleImage

IMPLEMENT_DYNAMIC(CSimpleImage, CWnd)

CSimpleImage::CSimpleImage(CWnd* pParent/*=NULL*/)
{
	int i;
	m_pParent = pParent;

	if (pParent)
		m_hParent = pParent->GetSafeHwnd();

	m_nIdxMk = 0;
	m_pMil = M_NULL;
	for (i = 0; i < MAX_DISP; i++)
	{
		m_pMilDispCad[i] = NULL;
		//m_pMilBufCad[i] = NULL;
		//m_pMilOvrCad[i] = NULL;
		//m_pMilDelOvrCad[i] = NULL;

		//m_pMilDispDef[i] = NULL;
		//m_pMilBufDef[i] = NULL;
		//m_pMilOvrDef[i] = NULL;
		//m_pMilDelOvrDef[i] = NULL;
	}

	//m_pMil = new CLibMil(this);
	//m_pMilDispCad[0] = new CLibMilDisp(this);

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
		//if (m_pMilBufCad[i])
		//{
		//	delete m_pMilBufCad[i];
		//	m_pMilBufCad[i] = NULL;
		//}
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
		//if (m_pMilBufDef[i])
		//{
		//	delete m_pMilBufDef[i];
		//	m_pMilBufDef[i] = NULL;
		//}
		//if (m_pMilDispDef[i])
		//{
		//	delete m_pMilDispDef[i];
		//	m_pMilDispDef[i] = NULL;
		//}
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
	CSimpleImage* pSimpleCamMaster = reinterpret_cast<CSimpleImage*>(lpContext);

	while (pSimpleCamMaster->ThreadIsAlive())
	{
		if (!pSimpleCamMaster->ProcImage())
			break;
	}

	pSimpleCamMaster->ThreadEnd();
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
		m_pMil->Copy(m_MilOvrCad[i + 1], m_MilOvrCad[i]);
		m_pMil->Copy(m_MilBufDef[i + 1], m_MilBufDef[i]);
	}
}

void CSimpleImage::DisplaySelect(int nKind, HWND hDispCtrl, CRect rtDispCtrl, int nIdx) // nKind : CAD_image[0], Defect_image[1]
{
	if (nKind == CAD_IMG)
	{
		//m_pMil->DisplaySelect(m_pMilDispCad[nIdx], m_MilBufCad[nIdx], hDispCtrl, rtDispCtrl);
		m_pMilDispCad[nIdx]->DisplaySelect(m_MilBufCad[nIdx], hDispCtrl, rtDispCtrl);
	}

	else if (nKind == DEF_IMG)
	{
		//m_pMil->DisplaySelect(m_pMilDispDef[nIdx], m_MilBufDef[nIdx], hDispCtrl, rtDispCtrl);
		//m_pMilDispDef[nIdx]->DisplaySelect(m_MilBufDef[nIdx], hDispCtrl);
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
