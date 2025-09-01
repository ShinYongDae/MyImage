
// MyImageDlg.h : 헤더 파일
//

#pragma once
#include "SimpleImage.h"

// CMyImageDlg 대화 상자
class CMyImageDlg : public CDialog
{
	CSimpleImage* m_pImage;
	int m_nIdxInfo;

	void Disp(int nSerial);
	void ShiftInfo();

// 생성입니다.
public:
	CMyImageDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CMyImageDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYIMAGE_DIALOG };
#endif

public:


protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
