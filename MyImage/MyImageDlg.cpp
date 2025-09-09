
// MyImageDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MyImage.h"
#include "MyImageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyImageDlg ��ȭ ����



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


// CMyImageDlg �޽��� ó����

BOOL CMyImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pImage = new CSimpleImage(this);


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMyImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
		AfxMessageBox(_T("CamMaster�� Spec���丮 Path�� �����Ǿ� ���� �ʽ��ϴ�."), MB_ICONWARNING | MB_OK);
		m_stSystem.sPathCamSpecDir = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Model Name"), NULL, szData, sizeof(szData), sPath))
		m_stLastJob.sModel = CString(szData);
	else
	{
		AfxMessageBox(_T("ModelUp�� �����Ǿ� ���� �ʽ��ϴ�."), MB_ICONWARNING | MB_OK);
		m_stLastJob.sModel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot No"), NULL, szData, sizeof(szData), sPath))
		m_stLastJob.sLot = CString(szData);
	else
	{
		AfxMessageBox(_T("LotUp�� �����Ǿ� ���� �ʽ��ϴ�."), MB_ICONWARNING | MB_OK);
		m_stLastJob.sLot = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerUp Name"), NULL, szData, sizeof(szData), sPath))
		m_stLastJob.sLayerUp = CString(szData);
	else
	{
		AfxMessageBox(_T("LayerUp�� �����Ǿ� ���� �ʽ��ϴ�."), MB_ICONWARNING | MB_OK);
		m_stLastJob.sLayerUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerDn Name"), NULL, szData, sizeof(szData), sPath))
		m_stLastJob.sLayerUp = CString(szData);
	else
	{
		AfxMessageBox(_T("LayerUp�� �����Ǿ� ���� �ʽ��ϴ�."), MB_ICONWARNING | MB_OK);
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