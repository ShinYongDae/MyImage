
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
}

CMyImageDlg::~CMyImageDlg()
{
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
	m_pImage = new CSimpleImage(this);


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
	CString str;
	COLORREF rgbDef;

	for (int i = 0; i < MAX_DISP - 1; i++)
	{
		//str = myStcDefInfo[i + 1].GetText();
		//rgbDef = myStcDefInfo[i + 1].GetBkColor();
		//myStcDefInfo[i].SetText(str);
		//myStcDefInfo[i].SetBkColor(rgbDef);
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
			nCtrlIdCad = IDC_PIC_CAD_012;
			nCtrlIdDef = IDC_PIC_DEF_012;
			break;
		}

		hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
		GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
#ifdef USE_VISION
		pView->m_pVision[0]->DisplaySelect(CAD_IMG, hW, rt, nIdxMkInfo);
#endif
		hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
		GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
#ifdef USE_VISION
		pView->m_pVision[0]->DisplaySelect(DEF_IMG, hW, rt, nIdxMkInfo);
#endif
	}
}