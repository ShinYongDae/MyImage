#pragma once

#include <thread>

#include "LibMil.h"
#include "LibMilDisp.h"

#include "Vic7defs.h"
#pragma comment (lib, "lib/vic64.lib")

#define MAX_DISP	12
#define DISP_SIZE_X	100
#define DISP_SIZE_Y	100
enum IMG_KIND { CAD_IMG = 0, DEF_IMG = 1 };

#define MAX_NodeNumX				100		//200
#define MAX_NodeNumY				100		//200
#define MAX_NMSWATH					100
#define RGN_STRIP_VARIABLE_NUM		23		// REGION_STRIP 구조체에서 배열을 제외한 변수의 갯수

#define  MAX_PROCNODENUM			60    // Max ProcNodeNum : 600mm/2.5um = 48 
#define  MAX_MOTOR_SWATH_NUM		35    // MAX Motor swath num : 500mm/(8k*2.5um) = 25  
#define  MAX_CELL_NUM				(MAX_PROCNODENUM * MAX_MOTOR_SWATH_NUM) * 3    // 4500 ea, 3IPU

typedef enum { PATH_ERROR = -1, PATH_NOT_FOUND, PATH_IS_FILE, PATH_IS_FOLDER }tpPath;

typedef struct {
	int nMode;											// 0 : Full Mode, 1 : Strip Mode
	int nMSwath;										// 모터가 움직일 회수.
	int NodeNumX, NodeNumY;								// 각 축당 셀 갯수
	int PanelPixelX, PanelPixelY;						// 전체 판넬 이미지의 크기
	int OvrXPix, OvrYPix;								// 각 축으로 오버랩되는 부분의 크기
	int ProcSizeX, ProcSizeY;							// 각 축당 셀의 크기
	int nCell;											// 전체 셀 갯수
	int nIPU;											// AOI의 IPU 갯수
	int CameraValidPixel;								// 실제 검사하기 위해 사용되는 이미지의 X축 크기
	int nScanMarginX, nScanMarginY;						// 실제 사용되는 이미지에 각 축방향으로 더해지는 여분 이미지의 크기 Ex) nScanMarginX = (8192 ? CameraValidPixel)/2
	int nRegionX, nRegionY;								// Reserved Data
	int nCellDivideX, nCellDivideY;						// 한 스트립당 각 축 방향 셀 갯수
	int nCADPinPosPixX, nCADPinPosPixY;					// Pin Position X, y
	int nProcMarginX, nProcMarginY;						// Reserved Data
	int StPosX[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 왼쪽 윗부분의 X 위치
	int StPosY[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 왼쪽 윗부분의 Y 위치
	int EdPosX[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 오른쪽 아래부분의 X 위치
	int EdPosY[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 오른쪽 아래부분의 Y 위치
	int XSwathPixPos[MAX_NMSWATH];						// 모터가 움직이기 시작하는 위치
} REGION_STRIP;		//Total 100476 Byte

// CSimpleImage

class CSimpleImage : public CWnd
{
	DECLARE_DYNAMIC(CSimpleImage)

	CWnd* m_pParent;
	HWND m_hParent;

	CString m_sPath;
	BOOL CreateWndForm(DWORD dwStyle);

	BOOL m_bThreadAlive, m_bThreadStateEnd;
	std::thread t1;
	void ThreadStart();
	void ThreadStop();

	int m_nIdxMk;
	CLibMil *m_pMil;
	CLibMilDisp *m_pMilDispCad[MAX_DISP], *m_pMilDispDef[MAX_DISP];
	MIL_ID m_MilBufCad[MAX_DISP];// , m_MilOvrCad[MAX_DISP];
	MIL_ID m_MilBufDef[MAX_DISP];// , m_MilOvrDef[MAX_DISP];

	void ShiftInfo();
	void DisplaySelect(int nKind, HWND hDispCtrl, CRect rtDispCtrl, int nIdx); // nKind : CAD_image[0], Defect_image[1]

	BOOL m_bFromCamMst;
	void CropCadImg(int nIdxMkInfo, int nSerial, int nLayer, int nIdxDef);
	void CropCadImg(short cell, short cx, short cy, int BufID, int nLayer);
	BOOL SetCADCoord(int CellNum, int StX, int StY, int Coord, int nLayer);
	void LoadCADBuf(int CurrCell, long OrgStX, long OrgStY, long DesStX, long DesStY, long SizeX, long SizeY, int nLayer);
	BOOL VicGetImgInfo(UCHAR *pCADImg, TiffData &tdat);
	BOOL VicFileLoadFromMem(MIL_ID MilImage, UCHAR *pCADImg, TiffData &tdat);

	int *m_pCell;
	CPoint *m_pDefPos;
	REGION_STRIP* m_pCellRgn;
	MIL_ID m_MilBufCADTemp;

	int *m_pDefType;
	CString m_sPathCamSpecDir;
	UCHAR *m_pCADCellImg[MAX_CELL_NUM];
	int   CellInspID[MAX_CELL_NUM];
	MIL_ID m_MilCADImgBuf;

	BOOL LoadMstInfo();
	//BOOL LoadMasterSpec();
	BOOL LoadStripRgnFromCam();
	void InitCADBuf();
	void LoadCadImg();
	BOOL DirectoryExists(LPCTSTR szPath);
	void DeleteFileInFolder(CString sPathDir);
	int CheckPath(CString strPath);
	void CADLinkImgFree();
	BOOL CADImgBufAlloc(TCHAR *strCADImg, int CellNum, BOOL bOppLayerF);

public:
	CSimpleImage(CWnd* pParent = NULL);
	virtual ~CSimpleImage();
	static void ProcThrd(const LPVOID lpContext);

public:
	void SelDispCad(HWND hDispCtrl, CRect rtDispCtrl, int nIdx);
	void SelDispDef(HWND hDispCtrl, CRect rtDispCtrl, int nIdx);
	void FreeDispCad(HWND hDispCtrl, CRect rtDispCtrl, int nIdx);
	void ShiftDisp();
	void ShowDispCad(int nIdxMkInfo, int nSerial, int nDefPcs);
	void ShowDispDef(int nIdxMkInfo, int nSerial, int nDefPcs);
	void SaveCadImg(int nIdxMkInfo, CString sPath);
	void ShowOvrCad(int nIdxMkInfo, int nSerial, int nIdxDef);
	BOOL Clear(int nIdxMkInfo);
	void SetFromCamMst(BOOL bOn);

protected:
	void ThreadEnd();
	BOOL ProcImage();
	BOOL ThreadIsAlive();


protected:
	DECLARE_MESSAGE_MAP()
};


