
// MyImageDlg.h : ��� ����
//

#pragma once
#include "SimpleImage.h"

// CMyImageDlg ��ȭ ����
class CMyImageDlg : public CDialog
{
	CSimpleImage* m_pImage;
	int m_nIdxInfo;

	void Disp(int nSerial);
	void ShiftInfo();

// �����Դϴ�.
public:
	CMyImageDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CMyImageDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYIMAGE_DIALOG };
#endif

public:


protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
