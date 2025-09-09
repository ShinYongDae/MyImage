
// MyImageDlg.h : ��� ����
//

#pragma once
#include "SimpleImage.h"

#define PATH_WORKING_INFO		_T("C:\\R2RSet\\WorkingInfo.ini")

struct stSystem
{
	CString sMcName;
	CString sPathCamSpecDir;

	CString sPathAoiUp, sPathAoiUpDts, sPathAoiUpCurrInfo, sPathAoiUpVrsData, sPathAoiUpOffset, sPathAoiUpDefImg;
	CString sPathAoiUpLocalSpec, sPathAoiUpCamInfo;
	CString sPathVrsShareUp, sPathVrsBufUp;
	CString sPathVsShareUp, sPathVsDummyBufUp;
	CString sPathAoiDn, sPathAoiDnDts, sPathAoiDnCurrInfo, sPathAoiDnVrsData, sPathAoiDnOffset, sPathAoiDnDefImg;
	CString sPathAoiDnLocalSpec, sPathAoiDnCamInfo;
	CString sPathVrsShareDn, sPathVrsBufDn;
	CString sPathVsShareDn, sPathVsDummyBufDn;

	CString sPathEng, sPathEngCurrInfo, sPathEngOffset, sPathMkCurrInfo, sPathMkCurrInfoBuf;
	CString sPathMkSignalInfo, sPathEngSignalInfo, sPathAoiUpStatusInfo, sPathAoiDnStatusInfo;
	CString sPathMkMenu01, sPathMkMenu03, sPathMkInfo, sPathMonDispMain;
	CString sPathMkWork;

	CString sPathOldFile, sPathItsFile, sPathIts;
	CString sIpPathOldFile, sIpPathItsFile, sIpPathIts;
	CString sPathSapp3;
	BOOL bSaveLog;
	BOOL bNoMk;	// 0 : ��ŷ���, 1 : �������
	CString sReViewMkLen;
	BOOL bReViewMk;

	CString sMaxDispDefImg;
	CString sCamSn[2];

	CString sIpClient[3];	// ID_SR1000W, ID_MDX2500, ID_PUNCH
	CString sIpServer[3];	// ID_SR1000W, ID_MDX2500, ID_ENGRAVE
	CString sPort[3];		// ID_SR1000W, ID_MDX2500, ID_ENGRAVE(ID_PUNCH)
	BOOL bSaveMkImg, bSaveGrabImg;
	BOOL bStripPcsRgnBin;
	BOOL bUseDTS, bUseITS;

	stSystem()
	{
		sMcName = _T("");
		sPathCamSpecDir = _T("");

		sPathAoiUp = _T(""); sPathAoiUpDts = _T(""); sPathAoiUpCurrInfo = _T(""); sPathAoiUpDefImg = _T("");
		sPathAoiUpOffset = _T(""); sPathAoiUpVrsData = _T(""); sPathAoiUpCamInfo = _T("");
		sPathAoiUpLocalSpec = _T("");
		sPathVrsShareUp = _T(""); sPathVrsBufUp = _T("");
		sPathVsShareUp = _T(""); sPathVsDummyBufUp = _T("");
		sPathAoiDn = _T(""); sPathAoiDnDts = _T(""); sPathAoiDnCurrInfo = _T(""); sPathAoiDnDefImg = _T("");
		sPathAoiDnOffset = _T(""); sPathAoiDnVrsData = _T(""); sPathAoiDnCamInfo = _T("");
		sPathAoiDnLocalSpec = _T("");
		sPathVrsShareDn = _T(""); sPathVrsBufDn = _T("");
		sPathVsShareDn = _T(""); sPathVsDummyBufDn = _T("");

		sPathEng = _T(""); sPathEngCurrInfo = _T(""); sPathEngOffset = _T(""); sPathMkCurrInfo = _T("");
		sPathMkSignalInfo = _T(""); sPathEngSignalInfo = _T("");
		sPathAoiUpStatusInfo = _T(""); sPathAoiDnStatusInfo = _T("");
		sPathMkCurrInfoBuf = _T(""); sPathMkMenu01 = _T(""); sPathMkMenu03 = _T(""); sPathMkInfo = _T(""); sPathMonDispMain = _T("");
		sPathMkWork = _T("");

		sPathOldFile = _T("");
		sPathItsFile = _T("");
		sPathIts = _T("");
		sIpPathOldFile = _T("");
		sIpPathItsFile = _T("");
		sIpPathIts = _T("");
		bSaveLog = FALSE;
		bNoMk = FALSE;	// 0 : ��ŷ���, 1 : �������
		sReViewMkLen = _T("");
		bReViewMk = FALSE;
		sPathSapp3 = _T("");

		sCamSn[0] = _T(""); sCamSn[1] = _T("");
		sMaxDispDefImg = _T("");

		//sIpClient[ID_SR1000W] = _T(""); sIpClient[ID_MDX2500] = _T(""); sIpClient[ID_PUNCH] = _T("");	// ID_SR1000W, ID_MDX2500, ID_PUNCH
		//sIpServer[ID_SR1000W] = _T(""); sIpServer[ID_MDX2500] = _T(""); sIpServer[ID_ENGRAVE] = _T("");	// ID_SR1000W, ID_MDX2500, ID_ENGRAVE
		//sPort[ID_SR1000W] = _T(""); sPort[ID_MDX2500] = _T(""); sPort[ID_ENGRAVE] = _T("");				// ID_SR1000W, ID_MDX2500, ID_ENGRAVE(ID_PUNCH)

		bSaveMkImg = FALSE;
		bSaveGrabImg = FALSE;
		bStripPcsRgnBin = FALSE;
		bUseDTS = FALSE;
		bUseITS = FALSE;
	}
};

struct stLastJob
{
	CString sProcessNum;
	CString sModel, sLayerUp, sLot, sSerialUp, sCompletedSerialUp;
	CString sLayerDn, sSerialDn, sCompletedSerialDn;
	CString sSerialEng;
	CString sInnerModel, sInnerLayerUp, sInnerLot;
	CString sInnerLayerDn;

	CString sSelUserName, sReelTotLen, sOnePnlLen;
	BOOL bLotSep;
	CString sLotSepLen, sLotCutPosLen;
	BOOL bTempPause;	// [0]:AOI-Up , [1]:AOI-Dn
	int nMergingLayer;
	CString sTempPauseLen, sLmtTotYld, sLmtPatlYld;
	CString sStripOutRatio, sCustomNeedRatio;
	BOOL bContFixDef;
	CString sNumRangeFixDef, sNumContFixDef, sUltraSonicCleannerStTim, sEngItsCode;
	BOOL bRclDrSen, bMkDrSen, bBufDrSen, bAoiUpDrSen, bAoiDnDrSen, bEngvDrSen, bUclDrSen;
	BOOL bUse380mm;
	BOOL bDispMkPcs, bStopFixDef, bMkSftySen, bAoiSftySen;
	CString sJogSpd, sLotSerial; //sLightVal, 
	BOOL bLightOn, bMkOnePnl, bAoiOnePnl, bEngraveOnePnl;
	BOOL bVerify, bReview;
	int nVerifyPeriod;
	CString sVerifyLen;
	//CString sDistAoiMk;
	CString sFurgeNum, sFurgeRun;
	CString sMkLastShot;
	CString sAoiLastShot[2]; // [Up/Dn]
	CString sPartialSpd;
	BOOL bOneMetal, bTwoMetal;
	BOOL bDualTest, bDualTestInner, bSampleTest, nTestMode;
	BOOL bCore150Recoiler, bCore150Uncoiler;
	CString sSampleTestShotNum;
	BOOL bUse2Layer;
	CString sEngraveOrderNum, sEngraveLastShot;
	CString sEngraveOrgX, sEngraveOrgY, sEngravePosOffsetX, sEngravePosOffsetY, sEngravePosTheta;
	int nAlignMethode;
	BOOL bUseAoiUpCleanRoler, bUseAoiDnCleanRoler;
	BOOL bUseEngraveUltrasonic, bUseAoiDnUltrasonic;
	BOOL bVelEngraveUltrasonic, bVelAoiDnUltrasonic;
	CString sCurrentShotNum, sSettingShotNum;
	int nAlarmTimePunch, nAlarmTimeAoi;
	BOOL bDispContRun, bDispLotEnd;
	BOOL bUseJudgeMkHisto;
	int nJudgeMkHistoRatio[2], nJudgeMkHistoWhite[2];

	stLastJob()
	{
		sProcessNum = _T("");
		//sModelUp = _T(""); sLayerUp = _T(""); sLotUp = _T(""); sSerialUp = _T(""); sCompletedSerialUp = _T("");
		//sModelDn = _T(""); sLayerDn = _T(""); sLotDn = _T(""); sSerialDn = _T(""); sCompletedSerialDn = _T("");
		sSerialEng = _T("");
		//sInnerModelUp = _T(""); sInnerLayerUp = _T(""); sInnerLotUp = _T("");
		//sInnerModelDn = _T(""); sInnerLayerDn = _T(""); sInnerLotDn = _T("");

		sSelUserName = _T(""); sReelTotLen = _T(""); sOnePnlLen = _T("");
		bLotSep = FALSE;
		sLotSepLen = _T(""); sLotCutPosLen = _T("");
		bTempPause = FALSE;
		nMergingLayer = 0; 	// [0]:AOI-Up , [1]:AOI-Dn
		sTempPauseLen = _T(""); sLmtTotYld = _T(""); sLmtPatlYld = _T("");
		sStripOutRatio = _T("20.0"); // Minimum 20%
		sCustomNeedRatio = _T("");
		bContFixDef = FALSE;
		sNumRangeFixDef = _T("");
		sNumContFixDef = _T("");
		sUltraSonicCleannerStTim = _T("5.0"); // AOI_Dn : MW05940, AOI_Up : MW05942
		bRclDrSen = FALSE; bMkDrSen = FALSE; bBufDrSen = FALSE; bAoiUpDrSen = FALSE; bAoiDnDrSen = FALSE; bEngvDrSen = FALSE; bUclDrSen = FALSE;
		bUse380mm = FALSE;
		bDispMkPcs = FALSE; bStopFixDef = FALSE; bMkSftySen = FALSE; bAoiSftySen = FALSE;
		sJogSpd = _T(""); sLotSerial = _T(""); //sLightVal=""); 
		bLightOn = FALSE; bMkOnePnl = FALSE; bAoiOnePnl = FALSE; bEngraveOnePnl = FALSE;
		bVerify = FALSE; bReview = FALSE;
		sVerifyLen = _T("");
		nVerifyPeriod = 0;
		//sDistAoiMk = _T("");
		sFurgeNum = _T(""); sFurgeRun = _T("");
		sMkLastShot = _T("0");
		sAoiLastShot[0] = _T("0");
		sAoiLastShot[1] = _T("0");
		sPartialSpd = _T("10");

		bOneMetal = FALSE; bTwoMetal = FALSE;
		bDualTest = TRUE; bDualTestInner = TRUE; bSampleTest = FALSE; nTestMode = 0;
		bCore150Recoiler = FALSE; bCore150Uncoiler = FALSE;
		sSampleTestShotNum = _T("");
		bUseEngraveUltrasonic = FALSE; bUseAoiDnUltrasonic = FALSE;
		bVelEngraveUltrasonic = FALSE; bVelAoiDnUltrasonic = FALSE;

		bUse2Layer = FALSE;

		sEngraveOrderNum = _T(""); sEngraveLastShot = _T("");
		sEngraveOrgX = _T(""); sEngraveOrgY = _T(""); sEngravePosOffsetX = _T(""); sEngravePosOffsetY = _T(""); sEngravePosTheta = _T("");
		//nAlignMethode = TWO_POINT;

		bUseAoiUpCleanRoler = FALSE; bUseAoiDnCleanRoler = FALSE;
		sEngItsCode = _T("");
		sCurrentShotNum = _T(""); sSettingShotNum = _T("");

		nAlarmTimePunch = 10800; nAlarmTimeAoi = 10800;
		bDispContRun = FALSE; bDispLotEnd = FALSE;

		bUseJudgeMkHisto = FALSE;
		nJudgeMkHistoRatio[0] = 50;
		nJudgeMkHistoRatio[1] = 50;
		nJudgeMkHistoWhite[0] = 240;
		nJudgeMkHistoWhite[1] = 240;
	}
};


// CMyImageDlg ��ȭ ����
class CMyImageDlg : public CDialog
{
	CSimpleImage* m_pImage;
	int m_nIdxInfo;
	stSystem m_stSystem;
	stLastJob m_stLastJob;

	BOOL LoadWorkingInfo();
	void Disp(int nSerial);
	void ShiftInfo();
	void SelDisp();

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
