// SimpleImage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyImage.h"
#include "SimpleImage.h"
#include "MyImageDlg.h"

#include <windowsx.h>	// GlobalFreePtr

unsigned char BitConvLUT2[256] = { 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 0x8, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8, 0x4, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 0xc, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 0x2, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 0xa, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa, 0x6, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6, 0xe, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 0x1, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1, 0x9, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 0x5, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 0xd, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd, 0x3, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 0xb, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 0x7, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7, 0xf, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff };

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

	m_bFromCamMst = FALSE;
	m_pCellRgn = NULL;
	m_pDefPos = NULL;
	m_sPathCamSpecDir = _T("\\\\gm-storage2\\Sp-C1iN\\");
	m_MilBufCADTemp = M_NULL;
	m_MilCADImgBuf = M_NULL;

	m_pDefPos = new CPoint[100];
	m_pCell = new INT[100];
	m_pDefType = new int[100];

	for (i = 0; i < MAX_CELL_NUM; i++)
		m_pCADCellImg[i] = NULL;

	LoadMstInfo();
	InitCADBuf();
}

CSimpleImage::~CSimpleImage()
{
	ThreadStop();
	Sleep(30);
	t1.join();

	if (m_pCellRgn)
	{
		delete m_pCellRgn;
		m_pCellRgn = NULL;
	}

	if (m_pDefPos)
	{
		delete[] m_pDefPos;
		m_pDefPos = NULL;
	}

	if (m_pCell)
	{
		delete[] m_pCell;
		m_pCell = NULL;
	}

	if (m_pDefType)
	{
		delete[] m_pDefType;
		m_pDefType = NULL;
	}


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

	CADLinkImgFree();

	if (m_MilBufCADTemp)
	{
		MbufFree(m_MilBufCADTemp);
		m_MilBufCADTemp = M_NULL;
	}

	if (m_MilCADImgBuf)
	{
		MbufFree(m_MilCADImgBuf);
		m_MilCADImgBuf = M_NULL;
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

void CSimpleImage::DisplaySelect(int nKind, HWND hDispCtrl, CRect rtDispCtrl, int nIdx) // nKind : CAD_image[0], Defect_image[1]
{
	MIL_ID SystemId = m_pMil->GetSystemId();

	if (nKind == CAD_IMG)
	{
		SelDispCad(hDispCtrl, rtDispCtrl, nIdx);
	}

	else if (nKind == DEF_IMG)
	{
		SelDispDef(hDispCtrl, rtDispCtrl, nIdx);
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
}

BOOL CSimpleImage::Clear(int nIdxMkInfo)
{
	if (m_MilBufCad[nIdxMkInfo])
	{
		MbufClear(m_MilBufCad[nIdxMkInfo], (double)0);
		return TRUE;
	}
	else
		return FALSE;

	if (m_pMilDispCad[nIdxMkInfo])
	{
		m_pMilDispCad[nIdxMkInfo]->ClearOverlay();
		return TRUE;
	}
	else
		return FALSE;

	if (m_MilBufDef[nIdxMkInfo])
	{
		MbufClear(m_MilBufDef[nIdxMkInfo], (double)0);
		return TRUE;
	}
	else
		return FALSE;
}

void CSimpleImage::ShowDispCad(int nIdxMkInfo, int nSerial, int nDefPcs) // From 0 To 12...for Screen display.
{
	if (m_bFromCamMst)
	{
		int nLayer = 0;
		CropCadImg(nIdxMkInfo, nSerial, nLayer, nDefPcs);
	}
	else
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

void CSimpleImage::ShowOvrCad(int nIdxMkInfo, int nSerial, int nIdxDef)
{
	CString str;
	str.Format(_T("%d-%d"), nSerial, nIdxDef);
	if(m_pMilDispCad[nIdxMkInfo])
		m_pMilDispCad[nIdxMkInfo]->ShowOverlay(str);
}

void CSimpleImage::ShiftInfo()
{
	for (int i = 0; i < MAX_DISP - 1; i++)
	{
		m_pMil->Copy(m_MilBufCad[i + 1], m_MilBufCad[i]);
		m_pMil->Copy(m_pMilDispCad[i + 1]->GetOverlayId(), m_pMilDispCad[i]->GetOverlayId());
		m_pMil->Copy(m_MilBufDef[i + 1], m_MilBufDef[i]);
		//m_pMil->Copy(m_MilOvrDef[i + 1], m_MilOvrDef[i]);
	}
}

void CSimpleImage::SetFromCamMst(BOOL bOn)
{
	m_bFromCamMst = bOn;
}

void CSimpleImage::CropCadImg(int nIdxMkInfo, int nSerial, int nLayer, int nIdxDef)
{
	short cell, cx, cy;
	int nIdx;

	if (!m_pCell || !m_pDefPos || !m_pCellRgn)
		return;

	cell = m_pCell[nIdxDef];									// BufIdx[], DefIdx[]
	//cx = m_pDefPos[nIdxDef].x - m_pCellRgn->StPosX[cell];		// BufIdx[], DefIdx[]
	//cy = m_pDefPos[nIdxDef].y - m_pCellRgn->StPosY[cell];		// BufIdx[], DefIdx[]
	cx = 922 * (nIdxDef + 1);
	cy = 371 * (nIdxDef + 1);
	CropCadImg(cell, cx, cy, nIdxMkInfo, nLayer);
}

void CSimpleImage::CropCadImg(short cell, short cx, short cy, int BufID, int nLayer)
{
	CString str;

	int CellX, CellY;
	BOOL IsLeftSwath, IsRightSwath, IsTopNode, IsBottomNode;	// 패널 이미지의 가장자리부분을 나타냄.
	BOOL IsUpNode, IsDownNode;									// 스트립의 상하 가장자리부분을 나타냄.
	int dx, dy;
	int stx, sty, edx, edy;
	int CelNum;

	REGION_STRIP *pCellRgn;


	pCellRgn = m_pCellRgn;
	CellX = cell / m_pCellRgn->NodeNumY;
	CellY = cell % m_pCellRgn->NodeNumY;


	dx = DISP_SIZE_X;
	dy = DISP_SIZE_Y;

	stx = cx - dx / 2;
	sty = cy - dy / 2;
	edx = cx + dx / 2;
	edy = cy + dy / 2;

	IsUpNode = 0;
	IsDownNode = 0;

	if (pCellRgn->NodeNumY > 0)
	{
		IsLeftSwath = (CellX == 0 ? 1 : 0);
		IsRightSwath = ((CellX + 1) == pCellRgn->NodeNumX ? 1 : 0);
		IsTopNode = (CellY == 0 ? 1 : 0);
		IsBottomNode = ((CellY + 1) == pCellRgn->NodeNumY ? 1 : 0);

		IsUpNode = (CellY % pCellRgn->nCellDivideY == 0 ? 1 : 0);
		IsDownNode = ((CellY % pCellRgn->nCellDivideY) == (pCellRgn->nCellDivideY - 1) ? 1 : 0);
	}
	else
	{
		AfxMessageBox(_T("Invalid CAD Img Info"));
		return;
	}

	if (stx < 0)
	{
		if (sty < 0)
		{
			if (IsLeftSwath && IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 1, nLayer);
				CelNum = 1;
			}
			else if (IsLeftSwath)
			{
				if (IsUpNode)
				{
					SetCADCoord(cell, stx, sty, 1, nLayer);
					CelNum = 1;
					if (0 <= (pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]))
					{
						if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < -sty)
						{
							SetCADCoord(cell, stx, sty, 31, nLayer);
							CelNum = 2;
						}
					}
					else if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < 0)
					{
						SetCADCoord(cell, stx, sty, 41, nLayer);
						CelNum = 2;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 1, nLayer);
					SetCADCoord(cell, stx, sty, 10, nLayer);
					CelNum = 2;
				}
			}
			else if (IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 1, nLayer);
				SetCADCoord(cell, stx, sty, 24, nLayer);
				CelNum = 2;
			}
			else
			{
				if (IsUpNode)
				{
					SetCADCoord(cell, stx, sty, 1, nLayer);
					SetCADCoord(cell, stx, sty, 24, nLayer);
					CelNum = 2;
					if (0 <= (pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]))
					{
						if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < -sty)
						{
							SetCADCoord(cell, stx, sty, 30, nLayer);
							SetCADCoord(cell, stx, sty, 31, nLayer);
							CelNum = 4;
						}
					}
					else if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < 0)
					{
						SetCADCoord(cell, stx, sty, 40, nLayer);
						SetCADCoord(cell, stx, sty, 41, nLayer);
						CelNum = 4;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 1, nLayer);
					SetCADCoord(cell, stx, sty, 9, nLayer);
					SetCADCoord(cell, stx, sty, 10, nLayer);
					SetCADCoord(cell, stx, sty, 24, nLayer);
					CelNum = 4;
				}
			}
		}
		else if (edy > pCellRgn->ProcSizeY)
		{
			if (IsLeftSwath && IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 7, nLayer);
				CelNum = 1;
			}
			else if (IsLeftSwath)
			{
				if (IsDownNode)
				{
					SetCADCoord(cell, stx, sty, 7, nLayer);
					CelNum = 1;
					if (0 <= (pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]))
					{
						if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < (DISP_SIZE_Y - (pCellRgn->ProcSizeY - sty)))
						{
							SetCADCoord(cell, stx, sty, 26, nLayer);
							CelNum = 2;
						}
					}
					else if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < 0)
					{
						SetCADCoord(cell, stx, sty, 36, nLayer);
						CelNum = 2;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 7, nLayer);
					SetCADCoord(cell, stx, sty, 20, nLayer);
					CelNum = 2;
				}
			}
			else if (IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 7, nLayer);
				SetCADCoord(cell, stx, sty, 22, nLayer);
				CelNum = 2;
			}
			else
			{
				if (IsDownNode)
				{
					SetCADCoord(cell, stx, sty, 7, nLayer);
					SetCADCoord(cell, stx, sty, 22, nLayer);
					CelNum = 2;
					if (0 <= (pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]))
					{
						if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < (DISP_SIZE_Y - (pCellRgn->ProcSizeY - sty)))
						{
							SetCADCoord(cell, stx, sty, 25, nLayer);
							SetCADCoord(cell, stx, sty, 26, nLayer);
							CelNum = 4;
						}
					}
					else if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < 0)
					{
						SetCADCoord(cell, stx, sty, 35, nLayer);
						SetCADCoord(cell, stx, sty, 36, nLayer);
						CelNum = 4;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 7, nLayer);
					SetCADCoord(cell, stx, sty, 20, nLayer);
					SetCADCoord(cell, stx, sty, 21, nLayer);
					SetCADCoord(cell, stx, sty, 22, nLayer);
					CelNum = 4;
				}
			}
		}
		else
		{
			SetCADCoord(cell, stx, sty, 8, nLayer);
			CelNum = 1;
			if (!IsLeftSwath)
			{
				SetCADCoord(cell, stx, sty, 23, nLayer);
				CelNum = 2;
			}
		}
	}
	else if (edx > pCellRgn->ProcSizeX)
	{
		if (sty < 0)
		{
			if (IsRightSwath && IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 3, nLayer);
				CelNum = 1;
			}
			else if (IsRightSwath)
			{
				if (IsUpNode)
				{
					SetCADCoord(cell, stx, sty, 3, nLayer);
					CelNum = 1;
					if (0 <= (pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]))
					{
						if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < -sty)
						{
							SetCADCoord(cell, stx, sty, 33, nLayer);
							CelNum = 2;
						}
					}
					else if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < 0)
					{
						SetCADCoord(cell, stx, sty, 43, nLayer);
						CelNum = 2;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 3, nLayer);
					SetCADCoord(cell, stx, sty, 12, nLayer);
					CelNum = 2;
				}
			}
			else if (IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 3, nLayer);
				SetCADCoord(cell, stx, sty, 14, nLayer);
				CelNum = 2;
			}
			else
			{
				if (IsUpNode)
				{
					SetCADCoord(cell, stx, sty, 3, nLayer);
					SetCADCoord(cell, stx, sty, 14, nLayer);
					CelNum = 2;
					if (0 <= (pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]))
					{
						if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < -sty)
						{
							SetCADCoord(cell, stx, sty, 33, nLayer);
							SetCADCoord(cell, stx, sty, 34, nLayer);
							CelNum = 4;
						}
					}
					else if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < 0)
					{
						SetCADCoord(cell, stx, sty, 43, nLayer);
						SetCADCoord(cell, stx, sty, 44, nLayer);
						CelNum = 4;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 3, nLayer);
					SetCADCoord(cell, stx, sty, 12, nLayer);
					SetCADCoord(cell, stx, sty, 13, nLayer);
					SetCADCoord(cell, stx, sty, 14, nLayer);
					CelNum = 4;
				}
			}
		}
		else if (edy > pCellRgn->ProcSizeY)
		{
			if (IsRightSwath && IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 5, nLayer);
				CelNum = 1;
			}
			else if (IsRightSwath)
			{
				if (IsDownNode)
				{
					SetCADCoord(cell, stx, sty, 5, nLayer);
					CelNum = 1;
					if (0 <= (pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]))
					{
						if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < (DISP_SIZE_Y - (pCellRgn->ProcSizeY - sty)))
						{
							SetCADCoord(cell, stx, sty, 28, nLayer);
							CelNum = 2;
						}
					}
					else if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < 0)
					{
						SetCADCoord(cell, stx, sty, 38, nLayer);
						CelNum = 2;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 5, nLayer);
					SetCADCoord(cell, stx, sty, 18, nLayer);
					CelNum = 2;
				}
			}
			else if (IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 5, nLayer);
				SetCADCoord(cell, stx, sty, 16, nLayer);
				CelNum = 2;
			}
			else
			{
				if (IsDownNode)
				{
					SetCADCoord(cell, stx, sty, 5, nLayer);
					SetCADCoord(cell, stx, sty, 16, nLayer);
					CelNum = 2;
					if (0 <= (pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]))
					{
						if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < (DISP_SIZE_Y - (pCellRgn->ProcSizeY - sty)))
						{
							SetCADCoord(cell, stx, sty, 28, nLayer);
							SetCADCoord(cell, stx, sty, 29, nLayer);
							CelNum = 4;
						}

					}
					else if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < 0)
					{
						SetCADCoord(cell, stx, sty, 38, nLayer);
						SetCADCoord(cell, stx, sty, 39, nLayer);
						CelNum = 4;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 5, nLayer);
					SetCADCoord(cell, stx, sty, 16, nLayer);
					SetCADCoord(cell, stx, sty, 17, nLayer);
					SetCADCoord(cell, stx, sty, 18, nLayer);
					CelNum = 4;
				}
			}
		}
		else
		{
			SetCADCoord(cell, stx, sty, 4, nLayer);
			CelNum = 1;
			if (!IsRightSwath)
			{
				SetCADCoord(cell, stx, sty, 15, nLayer);
				CelNum = 2;
			}
		}
	}
	else
	{
		if (sty < 0)
		{
			if (IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 2, nLayer);
				CelNum = 1;
			}
			else if (IsUpNode)
			{
				SetCADCoord(cell, stx, sty, 2, nLayer);
				CelNum = 1;
				if (0 <= (pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]))
				{
					if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < -sty)
					{
						SetCADCoord(cell, stx, sty, 32, nLayer);
						CelNum = 2;
					}
				}
				else if ((pCellRgn->StPosY[cell] - pCellRgn->EdPosY[cell - 1]) < 0)
				{
					SetCADCoord(cell, stx, sty, 42, nLayer);
					CelNum = 2;
				}
			}
			else
			{
				SetCADCoord(cell, stx, sty, 2, nLayer);
				SetCADCoord(cell, stx, sty, 11, nLayer);
				CelNum = 2;
			}
		}
		else if (edy > pCellRgn->ProcSizeY)
		{
			if (IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 6, nLayer);
				CelNum = 1;
			}
			else if (IsDownNode)
			{
				SetCADCoord(cell, stx, sty, 6, nLayer);
				CelNum = 1;
				if (0 <= (pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]))
				{
					if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < (DISP_SIZE_Y - (pCellRgn->ProcSizeY - sty)))
					{
						SetCADCoord(cell, stx, sty, 27, nLayer);
						CelNum = 2;
					}
				}
				else if ((pCellRgn->StPosY[cell + 1] - pCellRgn->EdPosY[cell]) < 0)
				{
					SetCADCoord(cell, stx, sty, 37, nLayer);
					CelNum = 2;
				}
			}
			else
			{
				SetCADCoord(cell, stx, sty, 6, nLayer);
				SetCADCoord(cell, stx, sty, 19, nLayer);
				CelNum = 2;
			}
		}
		else
		{
			SetCADCoord(cell, stx, sty, 0, nLayer);
			CelNum = 1;
		}
	}

	MbufCopy(m_MilBufCADTemp, m_MilBufCad[BufID]);
}

BOOL CSimpleImage::SetCADCoord(int CellNum, int StX, int StY, int Coord, int nLayer)
{
	BOOL RValue;
	long OrginalX, OrginalY;
	long DestX, DestY;
	long SizeX, SizeY;
	long EmpStripThick;

	REGION_STRIP *pCellRgn;
	pCellRgn = m_pCellRgn;

	switch (Coord)
	{
	case 0:		// In Current Cell
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = StY;
		SizeX = DISP_SIZE_X;
		SizeY = DISP_SIZE_Y;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 1:		// Left Top Corner Inside
		if (StX >= 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = 0;
		OrginalY = 0;
		SizeX = DISP_SIZE_X + StX;
		SizeY = DISP_SIZE_Y + StY;
		DestX = -StX;
		DestY = -StY;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 2:		// Top Inside
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = 0;
		SizeX = DISP_SIZE_X;
		SizeY = DISP_SIZE_Y + StY;
		DestX = 0;
		DestY = -StY;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 3:		// Right Top Corner Inside
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = 0;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = DISP_SIZE_Y + StY;
		DestX = 0;
		DestY = -StY;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 4:		// Right Inside
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = StY;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = DISP_SIZE_Y;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 5:		// Right Bottom Corner Inside
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = StY;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = pCellRgn->ProcSizeY - StY;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 6:		// Bottom Inside
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = StY;
		SizeX = DISP_SIZE_X;
		SizeY = pCellRgn->ProcSizeY - StY;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 7:		// Left Bottom Corner Inside
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = 0;
		OrginalY = StY;
		SizeX = DISP_SIZE_X + StX;
		SizeY = pCellRgn->ProcSizeY - StY;
		DestX = -StX;
		DestY = 0;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 8:		// Left Inside
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = 0;
		OrginalY = StY;
		SizeX = DISP_SIZE_X + StX;
		SizeY = DISP_SIZE_Y;
		DestX = -StX;
		DestY = 0;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 9:		// Left Top Corner Outside
		if (StX >= 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix;
		OrginalY = pCellRgn->ProcSizeY + StY - pCellRgn->OvrYPix;
		SizeX = -StX;
		SizeY = -StY;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum - pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 10:	// Top of Left Top Corner Outside
		if (StX >= 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = 0;
		OrginalY = pCellRgn->ProcSizeY + StY - pCellRgn->OvrYPix;
		SizeX = DISP_SIZE_X + StX;
		SizeY = -StY;
		DestX = -StX;
		DestY = 0;
		LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 11:		// Top Outside
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = pCellRgn->ProcSizeY + StY - pCellRgn->OvrYPix;
		SizeX = DISP_SIZE_X;
		SizeY = -StY;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 12: 		// Top of Right Top Corner Outside
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = pCellRgn->ProcSizeY + StY - pCellRgn->OvrYPix;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = -StY;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 13:  	// Right Top Corner Outside
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = pCellRgn->ProcSizeY + StY - pCellRgn->OvrYPix;
		SizeX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		SizeY = -StY;
		DestX = pCellRgn->ProcSizeX - StX;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 14: 		// Right of Right Top Corner Outside
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = 0;
		SizeX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		SizeY =DISP_SIZE_Y + StY;
		DestX = pCellRgn->ProcSizeX - StX;
		DestY = -StY;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 15: 		// Right Outside
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = StY;
		SizeX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		SizeY = DISP_SIZE_Y;
		DestX = pCellRgn->ProcSizeX - StX;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 16: 		// Right of Right Bottom Corner Outside
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = StY;
		SizeX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		SizeY = pCellRgn->ProcSizeY - StY;
		DestX = pCellRgn->ProcSizeX - StX;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 17: 		// Right of Right Bottom Corner Outside
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = pCellRgn->OvrYPix;
		SizeX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = pCellRgn->ProcSizeX - StX;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 18: 		// Bottom of Right Bottom Corner Outside
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = pCellRgn->OvrYPix;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 19: 		// Bottom Outside
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = pCellRgn->OvrYPix;
		SizeX = DISP_SIZE_X;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 20: 		// Bottom of Left Bottom Corner Outside
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = 0;
		OrginalY = pCellRgn->OvrYPix;
		SizeX = DISP_SIZE_X + StX;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = -StX;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 21: 		// Left Bottom of Left Bottom Corner Outside
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix;
		OrginalY = pCellRgn->OvrYPix;
		SizeX = -StX;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum - pCellRgn->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 22: 		// Left Bottom of Left Bottom Corner Outside
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix;
		OrginalY = StY;
		SizeX = -StX;
		SizeY = pCellRgn->ProcSizeY - StY;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum - pCellRgn->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 23: 		// Left Bottom of Left Bottom Corner Outside
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix;
		OrginalY = StY;
		SizeX = -StX;
		SizeY = DISP_SIZE_Y;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum - pCellRgn->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 24: 		// Left Bottom of Left Bottom Corner Outside
		if (StX >= 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix;
		OrginalY = 0;
		SizeX = -StX;
		SizeY = DISP_SIZE_Y + StY;
		DestX = 0;
		DestY = -StY;
		LoadCADBuf(CellNum - pCellRgn->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 25: 		// Left Bottom of Left Bottom Corner Outside for Strip Mode
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum + 1] - pCellRgn->EdPosY[CellNum];
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix;
		OrginalY = 0;
		SizeX = -StX;
		SizeY = DISP_SIZE_Y - EmpStripThick - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = EmpStripThick + (pCellRgn->ProcSizeY - StY);
		LoadCADBuf(CellNum - pCellRgn->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 26: 		// Bottom of Left Bottom Corner Outside for Strip Mode
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum + 1] - pCellRgn->EdPosY[CellNum];
		OrginalX = 0;
		OrginalY = 0;
		SizeX = DISP_SIZE_X + StX;
		SizeY = DISP_SIZE_Y - EmpStripThick - (pCellRgn->ProcSizeY - StY);
		DestX = -StX;
		DestY = EmpStripThick + (pCellRgn->ProcSizeY - StY);
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 27: 		// Bottom Outside for Strip Mode
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum + 1] - pCellRgn->EdPosY[CellNum];
		OrginalX = StX;
		OrginalY = 0;
		SizeX = DISP_SIZE_X;
		SizeY = DISP_SIZE_Y - EmpStripThick - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = EmpStripThick + (pCellRgn->ProcSizeY - StY);
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 28: 		// Bottom of Right Bottom Corner Outside for Strip Mode
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum + 1] - pCellRgn->EdPosY[CellNum];
		OrginalX = StX;
		OrginalY = 0;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = DISP_SIZE_Y - EmpStripThick - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = EmpStripThick + (pCellRgn->ProcSizeY - StY);
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 29: 		// Right Bottom of Right Bottom Corner Outside for Strip Mode
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum + 1] - pCellRgn->EdPosY[CellNum];
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = 0;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = DISP_SIZE_Y - EmpStripThick - (pCellRgn->ProcSizeY - StY);
		DestX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		DestY = EmpStripThick + (pCellRgn->ProcSizeY - StY);
		LoadCADBuf(CellNum + pCellRgn->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 30: 		// Left Top of Left Top Corner Outside for Strip Mode
		if (StX >= 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum] - pCellRgn->EdPosY[CellNum - 1];
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix; 
		OrginalY = pCellRgn->ProcSizeY + StY + EmpStripThick;
		SizeX = -StX;
		SizeY = -StY - EmpStripThick;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum - pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 31: 		// Top of Left Top Corner Outside for Strip Mode
		if (StX >= 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum] - pCellRgn->EdPosY[CellNum - 1];
		OrginalX = 0;
		OrginalY = pCellRgn->ProcSizeY + StY + EmpStripThick;
		SizeX = DISP_SIZE_X + StX;
		SizeY = -StY - EmpStripThick;
		DestX = -StX;
		DestY = 0;
		LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 32: 		// Top Outside for Strip Mode
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum] - pCellRgn->EdPosY[CellNum - 1];
		OrginalX = StX;
		OrginalY = pCellRgn->ProcSizeY + StY - EmpStripThick;
		SizeX = DISP_SIZE_X;
		SizeY = -StY - EmpStripThick;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 33: 		// Top of Right Top Corner Outside for Strip Mode
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum] - pCellRgn->EdPosY[CellNum - 1];
		OrginalX = StX;
		OrginalY = pCellRgn->ProcSizeY + StY - EmpStripThick;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = -StY - EmpStripThick;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 34: 		// Right Top of Right Top Corner Outside for Strip Mode
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->StPosY[CellNum] - pCellRgn->EdPosY[CellNum - 1];
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = pCellRgn->ProcSizeY + StY - EmpStripThick;
		SizeX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		SizeY = -StY - EmpStripThick;
		DestX = pCellRgn->ProcSizeX - StX;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 35: 		// Left Bottom of Left Bottom Corner Outside for Strip Mode
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix;
		OrginalY = EmpStripThick;
		SizeX = -StX;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum - pCellRgn->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 36: 		// Bottom of Left Bottom Corner Outside for Strip Mode
		if (StX >= 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		OrginalX = 0;
		OrginalY = EmpStripThick;
		SizeX = DISP_SIZE_X + StX;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = -StX;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 37: 		// Bottom Outside for Strip Mode
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		OrginalX = StX;
		OrginalY = EmpStripThick;
		SizeX = DISP_SIZE_X;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 38: 		// Bottom of Right Bottom Corner Outside for Strip Mode
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		OrginalX = StX;
		OrginalY = EmpStripThick;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = 0;
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 39: 		// Right Bottom of Right Bottom Corner Outside for Strip Mode
		if (StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = EmpStripThick;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = DISP_SIZE_Y - (pCellRgn->ProcSizeY - StY);
		DestX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		DestY = pCellRgn->ProcSizeY - StY;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 40: 		// Left Top of Left Top Corner Outside for Strip Mode
		if (StX >= 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		if (abs(StY) <= abs(EmpStripThick))
		{
			RValue = TRUE;
			break;
		}
		OrginalX = pCellRgn->ProcSizeX + StX - pCellRgn->OvrXPix;
		OrginalY = pCellRgn->ProcSizeY + StY;
		SizeX = -StX;
		SizeY = -StY - EmpStripThick;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 41: 		// Top of Left Top Corner Outside for Strip Mode
		if (StX >= 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		if (abs(StY) <= abs(EmpStripThick))
		{
			RValue = TRUE;
			break;
		}
		OrginalX = 0;
		OrginalY = pCellRgn->ProcSizeY + StY;
		SizeX = DISP_SIZE_X + StX;
		SizeY = -StY - EmpStripThick;
		DestX = -StX;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 42: 		// Top Outside for Strip Mode
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		if (abs(StY) <= abs(EmpStripThick))
		{
			RValue = TRUE;
			break;
		}
		OrginalX = StX;
		OrginalY = pCellRgn->ProcSizeY + StY;
		SizeX = DISP_SIZE_X;
		SizeY = -StY - EmpStripThick;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 43: 		// Top of Right Top Corner Outside for Strip Mode
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		if (abs(StY) <= abs(EmpStripThick))
		{
			RValue = TRUE;
			break;
		}
		OrginalX = StX;
		OrginalY = pCellRgn->ProcSizeY + StY;
		SizeX = pCellRgn->ProcSizeX - StX;
		SizeY = -StY - EmpStripThick;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 44: 		// Right Top of Right Top Corner Outside for Strip Mode
		if (StX < 0 || StY >= 0)
		{
			RValue = FALSE;
			break;
		}
		EmpStripThick = pCellRgn->EdPosY[CellNum] - pCellRgn->StPosY[CellNum + 1];
		if (abs(StY) <= abs(EmpStripThick))
		{
			RValue = TRUE;
			break;
		}
		OrginalX = pCellRgn->OvrXPix;
		OrginalY = pCellRgn->ProcSizeY + StY;
		SizeX = DISP_SIZE_X - (pCellRgn->ProcSizeX - StX);
		SizeY = -StY - EmpStripThick;
		DestX = pCellRgn->ProcSizeX - StX;
		DestY = 0;
		LoadCADBuf(CellNum + pCellRgn->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	default:
		break;
	}

	return(RValue);
}

void CSimpleImage::LoadCADBuf(int CurrCell, long OrgStX, long OrgStY, long DesStX, long DesStY, long SizeX, long SizeY, int nLayer)
{
	MIL_ID MilBufCADCld = M_NULL, MilBufCADTempCld = M_NULL;
	long OrgX, OrgY, DstX, DstY, SzX, SzY;
	TiffData tdat;

	REGION_STRIP *pCellRgn;
	UCHAR *pCADCellImg;

	pCellRgn = m_pCellRgn;
	pCADCellImg = m_pCADCellImg[CellInspID[CurrCell]];

	if (!m_MilCADImgBuf)
		return;

	if (VicFileLoadFromMem(m_MilCADImgBuf, pCADCellImg, tdat))
	{
		if ((OrgStX + SizeX) <= pCellRgn->ProcSizeX && (OrgStY + SizeY) <= pCellRgn->ProcSizeY)
		{
			if (SizeX < 0)
			{
				OrgX = OrgStX + SizeX;
				SzX = -SizeX;
			}
			else
			{
				OrgX = OrgStX;
				SzX = SizeX;
			}
			if (SizeY < 0)
			{
				OrgY = OrgStY + SizeY;
				SzY = -SizeY;
			}
			else
			{
				OrgY = OrgStY;
				SzY = SizeY;
			}
			MbufChild2d(m_MilCADImgBuf, OrgX, OrgY, SzX, SzY, &MilBufCADCld);
		}

		if ((DesStX + SizeX) <= DISP_SIZE_X && (DesStY + SizeY) <= DISP_SIZE_Y)
		{
			if (SizeX < 0)
			{
				DstX = DesStX + SizeX;
				SzX = -SizeX;
			}
			else
			{
				DstX = DesStX;
				SzX = SizeX;
			}
			if (SizeY < 0)
			{
				DstY = DesStY + SizeY;
				SzY = -SizeY;
			}
			else
			{
				DstY = DesStY;
				SzY = SizeY;
			}
			MbufChild2d(m_MilBufCADTemp, DstX, DstY, SzX, SzY, &MilBufCADTempCld);
		}
	}


	if (MilBufCADCld != M_NULL && MilBufCADTempCld != M_NULL)
		MbufCopy(MilBufCADCld, MilBufCADTempCld);

	if (MilBufCADTempCld)
	{
		MbufFree(MilBufCADTempCld);
		MilBufCADTempCld = M_NULL;
	}

	if (MilBufCADCld)
	{
		MbufFree(MilBufCADCld);
		MilBufCADCld = M_NULL;
	}
}

BOOL CSimpleImage::VicGetImgInfo(UCHAR *pCADImg, TiffData &tdat)
{
	int rcode = tiffinfofrombuffer(pCADImg, &tdat);
	if (rcode != NO_ERROR)  // Fill structure 
		return FALSE;
	return TRUE;
}

BOOL CSimpleImage::VicFileLoadFromMem(MIL_ID MilImage, UCHAR *pCADImg, TiffData &tdat)
{
	int rcode;
	long PitchByte;
	unsigned char *HostAddr;
	int mVicImgBitCnt;
	unsigned int i, j, k;
	imgdes image;

	rcode = tiffinfofrombuffer(pCADImg, &tdat);
	if (rcode != NO_ERROR) { // Fill structure 
		return FALSE;
	}

	rcode = allocimage(&image, tdat.width, tdat.length, tdat.vbitcount);
	if (rcode != NO_ERROR)
	{
		return FALSE;
	}

	rcode = loadtiffrombuffer(pCADImg, &image);
	if (rcode != NO_ERROR) // Free image on error
	{
		freeimage(&image);
		return FALSE;
	}

	mVicImgBitCnt = image.bmh->biBitCount;
	HostAddr = (unsigned char*)MbufInquire(MilImage, M_HOST_ADDRESS, M_NULL);
	PitchByte = MbufInquire(MilImage, M_PITCH_BYTE, M_NULL);

	k = tdat.length - 1;
	for (i = 0; i < tdat.length; i++)
	{
		for (j = 0; j < tdat.width / 8; j++)
		{
			HostAddr[k * PitchByte + j] = BitConvLUT2[image.ibuff[i * image.buffwidth + j]];
		}
		if ((tdat.width % 8) != 0)
		{
			HostAddr[k * PitchByte + j] = image.ibuff[i * image.buffwidth + j];
		}
		k--;
	}

	freeimage(&image);
	return TRUE;
}


BOOL CSimpleImage::LoadMstInfo()
{
	//LoadMasterSpec();
	if (LoadStripRgnFromCam())
	{
		LoadCadImg();
	}
	else
		return FALSE;

	return TRUE;
}

//BOOL CSimpleImage::LoadMasterSpec()
//{
//#ifdef USE_CAM_MASTER
//	//char szData[100];
//	TCHAR szData[100];
//	CString sMsg, sPath, sPathDir;
//	CFileFind findfile;
//	BOOL bRtn = TRUE;
//	sPathDir = m_sPathCamSpecDir;
//
//	sPath.Format(_T("%s%s\\%s.ini"), sPathDir,
//		m_sModel,
//		m_sLayer);
//
//
//	if (!findfile.FindFile(sPath))
//	{
//		sMsg.Format(_T("캠마스터 스팩 파일이 없습니다.\r\n%s"), sPath);
//		pView->MsgBox(sMsg);
//		// 		AfxMessageBox(sMsg);
//		return FALSE;
//	}
//
//	if (0 < ::GetPrivateProfileString(_T("MACHINE"), _T("PixelSize"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.dPixelSize = _tstof(szData); // [um]
//	else
//	{
//		MasterInfo.dPixelSize = 0.0; // [um]
//		bRtn = FALSE;
//	}
//
//	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("MASTERLOCATION1"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.strMasterLocation = CString(szData);
//	else
//	{
//		MasterInfo.strMasterLocation = _T("");
//		bRtn = FALSE;
//	}
//
//	if (0 < ::GetPrivateProfileString(_T("SPEC"), _T("Compression"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.nImageCompression = _tstoi(szData);
//	else
//		MasterInfo.nImageCompression = 1;
//
//	if (0 < ::GetPrivateProfileString(_T("SPEC"), _T("NumOfAlignPoint"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.nNumOfAlignPoint = _tstoi(szData);
//	else
//		MasterInfo.nNumOfAlignPoint = 2;
//
//	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("CADLINKLOCATION"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.strCADImgPath = CString(szData);
//	else
//		MasterInfo.strCADImgPath = _T("");
//
//	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("CadLinkLocationBackUp"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.strCADImgBackUpPath = CString(szData);
//	else
//		MasterInfo.strCADImgBackUpPath = _T("");
//
//	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("TopBottomInspection"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.bTwoMetalInspection = _tstoi(szData);
//	else
//		MasterInfo.bTwoMetalInspection = FALSE;
//
//	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("OppLayerName"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.strTwoMetalOppLayer = CString(szData);
//	else
//		MasterInfo.strTwoMetalOppLayer = _T("");
//
//	if (0 < ::GetPrivateProfileString(_T("ORIGIN COORD"), _T("ActionCode"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.nActionCode = _tstoi(szData);
//	else
//		MasterInfo.nActionCode = 0; // 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
//
//	if (0 < ::GetPrivateProfileString(_T("ORIGIN COORD"), _T("PX"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.dPinPosX = _tstof(szData); // [mm]
//	else
//		MasterInfo.dPinPosX = 0.0; // [mm]
//
//	if (0 < ::GetPrivateProfileString(_T("ORIGIN COORD"), _T("PY"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.dPinPosY = _tstof(szData); // [mm]
//	else
//		MasterInfo.dPinPosY = 0.0; // [mm]
//
//	if (0 < ::GetPrivateProfileString(_T("ORIGIN COORD"), _T("MX"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.dTestRgnLeft = _tstof(szData); // [mm]
//	else
//		MasterInfo.dTestRgnLeft = 0.0; // [mm]
//
//	if (0 < ::GetPrivateProfileString(_T("ORIGIN COORD"), _T("MY"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.dTestRgnTop = _tstof(szData); // [mm]
//	else
//		MasterInfo.dTestRgnTop = 0.0; // [mm]
//
//	if (0 < ::GetPrivateProfileString(_T("PANEL INFO"), _T("InspectionWidth"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.dTestRgnW = _tstof(szData); // [mm]
//	else
//		MasterInfo.dTestRgnW = 0.0; // [mm]
//
//	if (0 < ::GetPrivateProfileString(_T("PANEL INFO"), _T("InspectionHeight"), NULL, szData, sizeof(szData), sPath))
//		MasterInfo.dTestRgnH = _tstof(szData); // [mm]
//	else
//		MasterInfo.dTestRgnH = 0.0; // [mm]
//
//#else
//	MasterInfo.dPixelSize = 33.0;	// [um]
//#endif
//
//	return TRUE;
//}

BOOL CSimpleImage::LoadStripRgnFromCam()
{
	CFile file;
	int Rsize, RsizeStPosX, RsizeStPosY, RsizeEdPosX, RsizeEdPosY, RsizeXSwathPixPos;
	int SizeI, SizeIXSwathPixPos;
	TCHAR FileNCam[MAX_PATH];

	CString sPath, sMsg;
	if (m_sPathCamSpecDir.Right(1) != "\\")
		sPath.Format(_T("%s\\%s\\%s.mst"), m_sPathCamSpecDir, CAM_MODEL, CAM_LAYER_UP);
	else
		sPath.Format(_T("%s%s\\%s.mst"), m_sPathCamSpecDir, CAM_MODEL, CAM_LAYER_UP);

	wsprintf(FileNCam, TEXT("%s"), sPath);

	if (!file.Open(FileNCam, CFile::modeRead))
	{
		if (!file.Open(FileNCam, CFile::modeRead))
		{
			sMsg.Format(_T("캠마스터에서 해당모델의 작업정보가 없습니다.\r\n%s"), sPath);
			AfxMessageBox(sMsg);
			return(FALSE);
		}
	}

	if (m_pCellRgn)
		delete m_pCellRgn;
	m_pCellRgn = new REGION_STRIP;

	SizeI = sizeof(int) * RGN_STRIP_VARIABLE_NUM;
	Rsize = file.Read((void *)m_pCellRgn, SizeI);

	if (Rsize != SizeI)
	{
		AfxMessageBox(_T("MST File is incorrected."),MB_ICONSTOP|MB_OK);
		file.Close();
		return(FALSE);
	}

	SizeI = sizeof(int) * m_pCellRgn->NodeNumX * m_pCellRgn->NodeNumY;
	RsizeStPosX = file.Read((void *)&m_pCellRgn->StPosX, SizeI);
	RsizeStPosY = file.Read((void *)&m_pCellRgn->StPosY, SizeI);
	RsizeEdPosX = file.Read((void *)&m_pCellRgn->EdPosX, SizeI);
	RsizeEdPosY = file.Read((void *)&m_pCellRgn->EdPosY, SizeI);

	SizeIXSwathPixPos = sizeof(int) * m_pCellRgn->nMSwath;
	RsizeXSwathPixPos = file.Read((void *)&m_pCellRgn->XSwathPixPos, SizeIXSwathPixPos);

	if (RsizeStPosX != SizeI && RsizeStPosY != SizeI && RsizeEdPosX != SizeI && RsizeEdPosY != SizeI && RsizeXSwathPixPos != SizeIXSwathPixPos)
	{
		AfxMessageBox(_T("MST File is incorrected."),MB_ICONSTOP|MB_OK);
		file.Close();
		return(FALSE);
	}

	file.Close();
	return(TRUE);
}


BOOL CSimpleImage::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int CSimpleImage::CheckPath(CString strPath)
{
	DWORD dwAttr = GetFileAttributes(strPath);

	if (dwAttr == 0xffffffff)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND)
			return PATH_NOT_FOUND;
		return PATH_ERROR;
	}

	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) 
		return PATH_IS_FOLDER;
	return PATH_IS_FILE;
}

void CSimpleImage::DeleteFileInFolder(CString sPathDir)
{
	BOOL bRes;
	CFileFind ff;

	if (CheckPath(sPathDir) == PATH_IS_FILE)
	{
		DeleteFile(sPathDir);
		return;
	}

	if (sPathDir.Right(1) != _T("\\"))
		sPathDir.Append(_T("\\"));	//sPathDir += _T("\\");

	sPathDir.Append(_T("*.*"));	//sPathDir += _T("*.*");

	bRes = ff.FindFile(sPathDir);

	while (bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
			sPathDir = ff.GetFilePath();
			DeleteFileInFolder(sPathDir);
		}
		else
			DeleteFileInFolder(ff.GetFilePath());
	}

	ff.Close();
}

void CSimpleImage::LoadCadImg()
{
	BOOL prcStopF = FALSE;
	TCHAR FileNLoc[MAX_PATH];
	CString StrDirectoryPath;
	CFileFind CADFindFile;
	CString strFileNCam;
	int nStripCell;
	int i;
	int CurrSwath;
	int SwathCellNum, SwathNumX;
	int CellLoadID[MAX_CELL_NUM];

	TCHAR szData[100];
	CString strCADImgPath, sPath;
	sPath.Format(_T("%s%s\\%s.ini"), m_sPathCamSpecDir, CAM_MODEL, CAM_LAYER_UP);
	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("CADLINKLOCATION"), NULL, szData, sizeof(szData), sPath))
		strCADImgPath = CString(szData);
	else
		strCADImgPath = _T("");


	DWORD dwMilliseconds = 10;
	//DispMsg(_T("CAD 이미지를 다운로드 중입니다."), _T("CAD 이미지"), RGB_GREEN, DELAY_TIME_MSG);
	_stprintf(FileNLoc, _T("%s"), _T("C:\\R2RSet\\CAD"));

	if (!DirectoryExists(FileNLoc))
	{
		if (!CreateDirectory(FileNLoc, NULL))
		{
			AfxMessageBox(_T("Can not Create CAD Directory"), MB_ICONSTOP | MB_OK);
		}
	}
	else
	{
		StrDirectoryPath.Format(_T("%s"), FileNLoc);
		DeleteFileInFolder(FileNLoc);
	}
	CADLinkImgFree();

	// 한 스트립의 총 셀수 = 한 스트립의 x축상의 셀수 * 한 스트립의 y축상의 셀수.
	nStripCell = m_pCellRgn->nCellDivideX * m_pCellRgn->nCellDivideY;

	SwathCellNum = m_pCellRgn->nCellDivideX * m_pCellRgn->nCellDivideY;

	for (i = 0; i < (m_pCellRgn->NodeNumX * m_pCellRgn->NodeNumY); i++) // 한 Shot의 전체 셀 수.
	{
		SwathNumX = (i / m_pCellRgn->NodeNumY) / m_pCellRgn->nCellDivideX;
		CellInspID[i] = (i / m_pCellRgn->NodeNumY) * m_pCellRgn->nCellDivideY + (i % m_pCellRgn->nCellDivideY) - (SwathCellNum * SwathNumX);
	}

	for (i = 0; i < (m_pCellRgn->nCellDivideX * m_pCellRgn->nCellDivideY); i++) // 한 Strip의 전체 셀 수.
	{
		CurrSwath = i / m_pCellRgn->nCellDivideY;
		CellLoadID[i] = (i % m_pCellRgn->nCellDivideY) + (CurrSwath * m_pCellRgn->NodeNumY);
	}

	int Cell;
	TCHAR FileNCam[MAX_PATH];

	for (Cell = 0; Cell < nStripCell; Cell++) // 한 스트립의 총 셀수.
	{
		// CAM-Master File Copy and Local File Load
		strFileNCam.Format(_T("%s\\CORG%c%d.TIF"), strCADImgPath, 'O', CellLoadID[Cell]);
		_stprintf(FileNCam, _T("%s"), strFileNCam);
		_stprintf(FileNLoc, _T("C:\\R2RSet\\CAD\\CORG%c%d.TIF"), 'O', CellLoadID[Cell]);

		if (!CopyFile((LPCTSTR)FileNCam, (LPCTSTR)FileNLoc, FALSE))
		{
			if (!CopyFile((LPCTSTR)FileNCam, (LPCTSTR)FileNLoc, FALSE))
			{
				if (!CopyFile((LPCTSTR)FileNCam, (LPCTSTR)FileNLoc, FALSE))
				{
					prcStopF = TRUE;
					break;
				}
			}
		}

		CADImgBufAlloc(FileNLoc, Cell, FALSE);
	}

	if (!prcStopF)
		AfxMessageBox(_T("CAD 이미지 다운로드를 완료하였습니다."));
	else
		AfxMessageBox(_T("CAD 이미지 다운로드를 실패하였습니다."));
}

void CSimpleImage::InitCADBuf()
{
	REGION_STRIP *pCellRgn;
	pCellRgn = m_pCellRgn;

	if (m_MilCADImgBuf)
		MbufFree(m_MilCADImgBuf);
	if (pCellRgn)
	{
		if (m_pMil)
			MbufAlloc2d(m_pMil->GetSystemId(), pCellRgn->ProcSizeX, pCellRgn->ProcSizeY, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilCADImgBuf);
	}

	if (m_MilBufCADTemp)
		MbufFree(m_MilBufCADTemp);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemId(), DISP_SIZE_X, DISP_SIZE_Y, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilBufCADTemp);

	int i;
	//for (i = 0; i < MAX_CELL_NUM; i++)
	//	m_pCADCellImg[i] = NULL;

	for (i = 0; i < MAX_DISP; i++)
	{
		m_pCell[i] = (i + 1) * 2;
		m_pDefPos[i].x = 9228 * (i) - 0;
		m_pDefPos[i].y = 3712 * (i) - 0;
	}
}

void CSimpleImage::CADLinkImgFree()
{
	int i;

	for (i = 0; i < MAX_CELL_NUM; i++)
	{
		if (m_pCADCellImg[i])
		{
			GlobalFreePtr(m_pCADCellImg[i]);
			m_pCADCellImg[i] = NULL;
		}
	}
}

BOOL CSimpleImage::CADImgBufAlloc(TCHAR *strCADImg, int CellNum, BOOL bOppLayerF)
{
	long CADFileSize[MAX_CELL_NUM];
	CFile file;
	int RSize;

	if (!file.Open(strCADImg, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(strCADImg, CFile::modeRead | CFile::typeBinary))
		{
			return(FALSE);
		}
	}

	CADFileSize[CellNum] = file.GetLength();
	if (CADFileSize[CellNum] != 0)
		m_pCADCellImg[CellNum] = (UCHAR *)GlobalAllocPtr(GMEM_MOVEABLE, CADFileSize[CellNum]);

	RSize = file.Read((void *)m_pCADCellImg[CellNum], CADFileSize[CellNum]);
	if (RSize != CADFileSize[CellNum])
	{
		file.Close();
		return(FALSE);
	}

	file.Close();
	return TRUE;
}
