// SimpleImage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyImage.h"
#include "SimpleImage.h"


// CSimpleImage

IMPLEMENT_DYNAMIC(CSimpleImage, CWnd)

CSimpleImage::CSimpleImage(CWnd* pParent/*=NULL*/)
{
	m_pParent = pParent;
	if (pParent)
		m_hParent = pParent->GetSafeHwnd();

	m_nIdxMk = 0;

	CreateWndForm(WS_CHILD | WS_OVERLAPPED);
	ThreadStart();
}

CSimpleImage::~CSimpleImage()
{
	ThreadStop();
	Sleep(30);
	t1.join();
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
		m_Mil.Copy(m_MilBufCad[i + 1], m_MilBufCad[i]);
		m_Mil.Copy(m_MilOvrCad[i + 1], m_MilOvrCad[i]);
		m_Mil.Copy(m_MilBufDef[i + 1], m_MilBufDef[i]);
	}
}
