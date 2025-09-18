
// MyImageDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MyImage.h"
#include "MyImageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyImageDlg 대화 상자



CMyImageDlg::CMyImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MYIMAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImage = NULL;
	m_nIdxInfo = 0;

	m_bTHREAD_DISP_DEF = FALSE;
	m_nStepTHREAD_DISP_DEF = 0;
	m_nIdxMkInfo = 0;
	m_nIdxDef = 0;
	m_nDef = 0;
	m_nSerial = 0;
}

CMyImageDlg::~CMyImageDlg()
{
	ThreadStop();
	Sleep(30);
	t1.join();

	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

void CMyImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyImageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyImageDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyImageDlg 메시지 처리기

BOOL CMyImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	LoadWorkingInfo();

	m_pImage = new CSimpleImage(this);
	SelDisp();

	ThreadStart();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyImageDlg::ProcThrd(const LPVOID lpContext)
{
	CMyImageDlg* pMyImageDlg = reinterpret_cast<CMyImageDlg*>(lpContext);

	while (pMyImageDlg->ThreadIsAlive())
	{
		if (!pMyImageDlg->ProcDlg())
			break;
	}

	pMyImageDlg->ThreadEnd();
}

BOOL CMyImageDlg::ProcDlg()
{
	if (m_bTHREAD_DISP_DEF)
	{
		DispDefImg();						// Display sequence control
		DispMkInfo(m_nSerialDispMkInfo);	// Display image and info
		Sleep(0);
	}
	else
		Sleep(30);
	return TRUE;
}

void CMyImageDlg::ThreadStart()
{
	m_bThreadStateEnd = FALSE;
	m_bThreadAlive = TRUE;
	t1 = std::thread(ProcThrd, this);
}

void CMyImageDlg::ThreadStop()
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

void CMyImageDlg::ThreadEnd()
{
	m_bThreadStateEnd = TRUE;
}

BOOL CMyImageDlg::ThreadIsAlive()
{
	return m_bThreadAlive;
}


void CMyImageDlg::DispMkInfo(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.3"));
		return;
	}

	int nIdxMkInfo = (m_nIdxMkInfo < MAX_DISP) ? m_nIdxMkInfo : (MAX_DISP - 1);

	int nDefImg = m_nIdxDef;
	int nTotDef = 12;

	if (m_nIdxDef < nTotDef)
	{
		if (m_nIdxMkInfo >= MAX_DISP)
			ShiftInfo();
		//m_pImage->ShowDispCad(nIdxMkInfo, nSerial, 0, m_nIdxDef[0]);
		//m_pImage->ShowOvrCad(nIdxMkInfo, nSerial);
		if (m_pImage)
		{
			m_pImage->ShowDispCad(nIdxMkInfo, nSerial, nDefImg);
			m_pImage->ShowDispDef(nIdxMkInfo, nSerial, nDefImg);
		}
		//nDefImg = pDoc->m_pPcr[0][nIdx]->m_pImg[m_nIdxDef[0]];
		//pView->m_pVision[0]->ShowDispDef(nIdxMkInfo, nSerial, 0, nDefImg);
		SaveCadImg(nSerial, nIdxMkInfo, nDefImg);
		ShowDefInfo(nIdxMkInfo);
		//SaveDefImgPosUp(nSerial, nIdxMkInfo, nDefImg);
		m_nIdxMkInfo++;
		m_nIdxDef++;
	}
	else
		m_nIdxMkInfo++;
}

void CMyImageDlg::DispDefImg()
{
	int nSerialL = 1; // 좌측 Camera
	int nSerialR = 2; // 우측 Camera
	CString sNewLot = _T("LotName");
	CString str;
	BOOL bNewModel = FALSE;
	int nNewLot = 0;
	int nBreak = 0;
	int nLastShot = 0;

	switch (m_nStepTHREAD_DISP_DEF)
	{
	// CopyDefImg Start ============================================
	case 0:
		m_nStepTHREAD_DISP_DEF++;
		m_nSerialDispMkInfo = nSerialL;	// m_nSerial
		if (nSerialL > 0)
		{
			if (!CopyDefImg(nSerialL)) // 좌측 Camera
			{
				AfxMessageBox(_T("정지 - 좌측 Camera"));
				break;
			}

			if (nSerialR > 0)
			{
				if (!CopyDefImg(nSerialR)) // 우측 Camera
				{
					AfxMessageBox(_T("정지 - 우측 Camera"));
					break;
				}
			}
		}
		break;
	case 1:
		m_nStepTHREAD_DISP_DEF++;
		if (nSerialL > 0)
		{
			if (!CopyCadImg(nSerialL)) // 좌측 Camera
			{
				AfxMessageBox(_T("정지 - 좌측 Camera"));
				break;
			}

			if (nSerialR > 0)
			{
				if (!CopyCadImg(nSerialR)) // 우측 Camera
				{
					AfxMessageBox(_T("정지 - 우측 Camera"));
					break;
				}
			}
		}
		break;
	case 2:
		if (IsDoneDispMkInfo())	 // Check 불량이미지(이전) Display End
		{
			m_nDef = 0;
			if (nSerialL > 0)
			{
				m_nSerial = nSerialL;
				DispMkInfo();	// 불량이미지(좌) Display Start
			}
			m_nStepTHREAD_DISP_DEF++;
		}
		break;

	case 3:
		if (IsDoneDispMkInfo())	 // Check 불량이미지(좌) Display End
		{
			m_nDef = 0;
			m_nSerialDispMkInfo = nSerialR;	// m_nSerial
			if (nSerialR > 0) // 우측 Camera
			{
				m_nSerial = nSerialR;
				DispMkInfo();	// 불량이미지(우) Display Start
			}
			m_nStepTHREAD_DISP_DEF++;
		}
		break;
	case 4:
		if (IsDoneDispMkInfo())	 // Check 불량이미지(우) Display End
		{
			m_nDef = 0;
			m_nStepTHREAD_DISP_DEF++;
		}
		break;
	case 5:
		m_bTHREAD_DISP_DEF = FALSE;
		break;
		// CopyDefImg End ============================================
	}

}

void CMyImageDlg::DispMkInfo()
{
	int nMaxMaxDispDefImg = 12;
	int nTotDef = 12;

	if (nMaxMaxDispDefImg > 0 && nTotDef - nMaxMaxDispDefImg > 0)
		m_nIdxDef = nTotDef - nMaxMaxDispDefImg; // 불량이미지 인덱스.
	else
		m_nIdxDef = 0; // 불량이미지 인덱스.

	if (m_nIdxMkInfo > MAX_DISP)
		m_nIdxMkInfo = MAX_DISP;

	m_nDef = nTotDef; // m_nDef : m_nIdxMkInfo + Display Def Num.
	//m_bTHREAD_DISP_DEF = TRUE;
	//m_nSerialDispMkInfo = 1;	// m_nSerial
}

BOOL CMyImageDlg::IsDoneDispMkInfo()
{
	return (m_nDef <= m_nIdxDef) ? TRUE : FALSE;
	//return (!m_bTHREAD_DISP_DEF);
}

BOOL CMyImageDlg::LoadWorkingInfo()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("CamSpecDir"), NULL, szData, sizeof(szData), sPath))
		m_stSystem.sPathCamSpecDir = CString(szData);
	else
	{
		AfxMessageBox(_T("CamMaster의 Spec디렉토리 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		m_stSystem.sPathCamSpecDir = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Model Name"), NULL, szData, sizeof(szData), sPath))
		m_stLastJob.sModel = CString(szData);
	else
	{
		AfxMessageBox(_T("ModelUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		m_stLastJob.sModel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot No"), NULL, szData, sizeof(szData), sPath))
		m_stLastJob.sLot = CString(szData);
	else
	{
		AfxMessageBox(_T("LotUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		m_stLastJob.sLot = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerUp Name"), NULL, szData, sizeof(szData), sPath))
		m_stLastJob.sLayerUp = CString(szData);
	else
	{
		AfxMessageBox(_T("LayerUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		m_stLastJob.sLayerUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerDn Name"), NULL, szData, sizeof(szData), sPath))
		m_stLastJob.sLayerUp = CString(szData);
	else
	{
		AfxMessageBox(_T("LayerUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		m_stLastJob.sLayerUp = CString(_T(""));
	}

	return TRUE;
}

void CMyImageDlg::Disp(int nSerial)
{
	if (m_nIdxInfo >= MAX_DISP)	ShiftInfo();

	if (m_pImage)
		m_pImage->DispMkInfo(nSerial);
}

void CMyImageDlg::ShiftInfo()
{
	int nCtrlId[2];
	CString str;
	COLORREF rgbDef;

	if (m_pImage)
		m_pImage->ShiftInfo();

	for (int i = 0; i < MAX_DISP - 1; i++)
	{
		switch (i)
		{
		case 0:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_001;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_002;
			break;
		case 1:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_002;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_003;
			break;
		case 2:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_003;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_004;
			break;
		case 3:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_004;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_005;
			break;
		case 4:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_005;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_006;
			break;
		case 5:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_006;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_007;
			break;
		case 6:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_007;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_008;
			break;
		case 7:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_008;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_009;
			break;
		case 8:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_009;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_010;
			break;
		case 9:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_010;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_011;
			break;
		case 10:
			nCtrlId[0] = IDC_EDIT_DEF_INFO_011;
			nCtrlId[1] = IDC_EDIT_DEF_INFO_12;
			break;
		case 11:
			return;
		}

		GetDlgItem(nCtrlId[1])->GetWindowText(str);
		GetDlgItem(nCtrlId[0])->SetWindowText(str);
	}
}

void CMyImageDlg::SelDisp()
{
	HWND hW;
	CRect rt;
	int nCtrlIdCad, nCtrlIdDef;

	for (int nIdxMkInfo = 0; nIdxMkInfo < MAX_DISP; nIdxMkInfo++)
	{
		switch (nIdxMkInfo)
		{
		case 0:
			nCtrlIdCad = IDC_PIC_CAD_001;
			nCtrlIdDef = IDC_PIC_DEF_001;
			break;
		case 1:
			nCtrlIdCad = IDC_PIC_CAD_002;
			nCtrlIdDef = IDC_PIC_DEF_002;
			break;
		case 2:
			nCtrlIdCad = IDC_PIC_CAD_003;
			nCtrlIdDef = IDC_PIC_DEF_003;
			break;
		case 3:
			nCtrlIdCad = IDC_PIC_CAD_004;
			nCtrlIdDef = IDC_PIC_DEF_004;
			break;
		case 4:
			nCtrlIdCad = IDC_PIC_CAD_005;
			nCtrlIdDef = IDC_PIC_DEF_005;
			break;
		case 5:
			nCtrlIdCad = IDC_PIC_CAD_006;
			nCtrlIdDef = IDC_PIC_DEF_006;
			break;
		case 6:
			nCtrlIdCad = IDC_PIC_CAD_007;
			nCtrlIdDef = IDC_PIC_DEF_007;
			break;
		case 7:
			nCtrlIdCad = IDC_PIC_CAD_008;
			nCtrlIdDef = IDC_PIC_DEF_008;
			break;
		case 8:
			nCtrlIdCad = IDC_PIC_CAD_009;
			nCtrlIdDef = IDC_PIC_DEF_009;
			break;
		case 9:
			nCtrlIdCad = IDC_PIC_CAD_010;
			nCtrlIdDef = IDC_PIC_DEF_010;
			break;
		case 10:
			nCtrlIdCad = IDC_PIC_CAD_011;
			nCtrlIdDef = IDC_PIC_DEF_011;
			break;
		case 11:
			nCtrlIdCad = IDC_PIC_CAD_12;
			nCtrlIdDef = IDC_PIC_DEF_12;
			break;
		}

		hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
		GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
		if(m_pImage)
			m_pImage->DisplaySelect(CAD_IMG, hW, rt, nIdxMkInfo);
		hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
		GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
		if (m_pImage)
			m_pImage->DisplaySelect(DEF_IMG, hW, rt, nIdxMkInfo);
	}
}

BOOL CMyImageDlg::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return ((dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CMyImageDlg::MakeImageDir(int nSerial)
{
	CString strDefImgPathS, strDefImgPathD, strMakeFolderPath;
	CString sPathOldFile = PATH_OLD_FILE;

	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s"), sPathOldFile, MODEL_NAME);
	else
		strMakeFolderPath.Format(_T("%s%s"), sPathOldFile, MODEL_NAME);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s"), sPathOldFile, MODEL_NAME, LOT_NAME);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s"), sPathOldFile, MODEL_NAME, LOT_NAME);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);


	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImagePos"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImagePos"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\CadImage"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\CadImage"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImagePos\\%d"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\CadImage\\%d"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\CadImage\\%d"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial);

	if (!DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);
}

BOOL CMyImageDlg::CopyCadImg(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error."));
		return FALSE;
	}

	CString strDefImgPathS, strDefImgPathD;
	int i;
	CFileFind finder;
	CString sTemp;

	CString sPathOldFile = PATH_OLD_FILE;
	CString sImgDataPath;
	sImgDataPath.Format(_T("%sCadImage"), PATH_IMG_VRS);

	MakeImageDir(nSerial);

	int nTotDef = 12;
	int nErrorCnt = 0;
	for (i = 0; i < nTotDef; i++)
	{
		int nDefImg = i;
		if (sImgDataPath.Right(1) != "\\")
			strDefImgPathS.Format(_T("%s\\%s\\%s\\%s\\%d\\%05d.tif"), sImgDataPath, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
		else
			strDefImgPathS.Format(_T("%s%s\\%s\\%s\\%d\\%05d.tif"), sImgDataPath, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);

		if (sPathOldFile.Right(1) != "\\")
			strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\CadImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
		else
			strDefImgPathD.Format(_T("%s%s\\%s\\%s\\CadImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
	
		if (finder.FindFile(strDefImgPathS))
		{
			if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					sTemp.Format(_T("%s \r\n: CAD Image File Copy Fail"), strDefImgPathS);
					AfxMessageBox(sTemp);
					return FALSE;
				}
			}
		}
		else
		{
			Sleep(30);
			if (nErrorCnt > 10)
			{
				nErrorCnt = 0;
				sTemp.Format(_T("%s \r\n: CAD Image File Not Exist"), strDefImgPathS);
				AfxMessageBox(sTemp);
				return TRUE;
			}
			else
			{
				nErrorCnt++;
				i--;
				continue;
			}
		}


		int nStrip = -1, nCol = -1, nRow = -1;

		if (sPathOldFile.Right(1) != "\\")
		{
			strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\CadImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
		}
		else
		{
			strDefImgPathD.Format(_T("%s%s\\%s\\%s\\CadImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
		}

		if (finder.FindFile(strDefImgPathS))
		{
			if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					sTemp.Format(_T("%s \r\n: CAD Image Position File Copy Fail"), strDefImgPathS);
					AfxMessageBox(sTemp);
					return FALSE;
				}
			}
		}
		else
		{
			Sleep(30);
			if (nErrorCnt > 10)
			{
				nErrorCnt = 0;
				sTemp.Format(_T("%s \r\n: CAD Image Position File Not Exist"), strDefImgPathS);
				AfxMessageBox(sTemp);
				return TRUE;
			}
			else
			{
				nErrorCnt++;
				i--;
				continue;
			}
		}
	}
	
	return TRUE;
}

BOOL CMyImageDlg::CopyDefImg(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error."));
		return FALSE;
	}

	CString strDefImgPathS, strDefImgPathD;
	int i;
	CFileFind finder;
	CString sTemp;

	CString sPathOldFile = PATH_OLD_FILE;
	CString sImgDataPath;
	sImgDataPath.Format(_T("%sDefImage"), PATH_IMG_VRS);


	MakeImageDir(nSerial);

	int nTotDef = 12;
	int nErrorCnt = 0;
	for (i = 0; i < nTotDef; i++)
	{
		int nDefImg = i;
		if (sImgDataPath.Right(1) != "\\")
			strDefImgPathS.Format(_T("%s\\%s\\%s\\%s\\%d\\%05d.tif"), sImgDataPath, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
		else
			strDefImgPathS.Format(_T("%s%s\\%s\\%s\\%d\\%05d.tif"), sImgDataPath, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);

		if (sPathOldFile.Right(1) != "\\")
			strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
		else
			strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);

		if (finder.FindFile(strDefImgPathS))
		{
			if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					sTemp.Format(_T("%s \r\n: Defect Image File Copy Fail"), strDefImgPathS);
					AfxMessageBox(sTemp);
					return FALSE;
				}
			}
		}
		else
		{
			Sleep(30);
			if (nErrorCnt > 10)
			{
				nErrorCnt = 0;
				sTemp.Format(_T("%s \r\n: Defect Image File Not Exist"), strDefImgPathS);
				AfxMessageBox(sTemp);
				return TRUE;
			}
			else
			{
				nErrorCnt++;
				i--;
				continue;
			}
		}


		int nStrip = -1, nCol = -1, nRow = -1;

		if (sPathOldFile.Right(1) != "\\")
		{
			strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
		}
		else
		{
			strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nDefImg);
		}

		if (finder.FindFile(strDefImgPathS))
		{
			if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					sTemp.Format(_T("%s \r\n: Defect Image Position File Copy Fail"), strDefImgPathS);
					AfxMessageBox(sTemp);
					return FALSE;
				}
			}
		}
		else
		{
			Sleep(30);
			if (nErrorCnt > 10)
			{
				nErrorCnt = 0;
				sTemp.Format(_T("%s \r\n: Defect Image Position File Not Exist"), strDefImgPathS);
				AfxMessageBox(sTemp);
				return TRUE;
			}
			else
			{
				nErrorCnt++;
				i--;
				continue;
			}
		}
	}

	return TRUE;
}


void CMyImageDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nStepTHREAD_DISP_DEF = 0;
	m_bTHREAD_DISP_DEF = TRUE;				// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
}

void CMyImageDlg::ShowDefInfo(int nIdx) // nIdx : 0 ~ 11 (12ea)
{
	CString str;
	int nDefCode, nStrip, nCol, nRow;
	COLORREF rgbDef;

	str.Format(_T("%d"), nIdx);
	GetDlgItem(IDC_EDIT_DEF_INFO_12)->GetWindowText(str);
}

void CMyImageDlg::SaveCadImg(int nSerial, int nIdxMkInfo, int nIdxImg) // (nSerial, 화면의 IDC 인덱스, 불량이미지 인덱스)
{
	MakeImageDir(nSerial);
	CString sPathOldFile = PATH_OLD_FILE;
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\CadImage\\%d\\%05d.tif"), sPathOldFile, MODEL_NAME, LOT_NAME, LAYER_NAME, nSerial, nIdxImg);

	if (m_pImage)
		m_pImage->SaveCadImg(nIdxMkInfo, sPath);
}
