#pragma once


#ifndef MIL_H
#define MIL_H
#include <mil.h>
#pragma comment (lib, "mil.lib")
#pragma comment (lib, "milcolor.lib")
#pragma comment (lib, "Milim.lib")
#pragma comment (lib, "Miledge.lib")
#pragma comment (lib, "MilMeas.lib")
#pragma comment (lib, "Milmetrol.lib")
#pragma comment (lib, "milBlob.lib")
#pragma comment (lib, "milcal.lib")
#pragma comment (lib, "Milcode.lib")
#pragma comment (lib, "Milreg.lib")
#pragma comment (lib, "MilStr.lib")
#pragma comment (lib, "milpat.lib")
#pragma comment (lib, "milmod.lib")
#pragma comment (lib, "milocr.lib")
#endif


#define DISPLAY_FIT_MODE_CENTERVIEW		0
#define DISPLAY_FIT_MODE_MIN_RATIO		1
#define DISPLAY_FIT_MODE_MAX_RATIO		2
#define DISPLAY_FIT_MODE_X_RATIO		3
#define DISPLAY_FIT_MODE_Y_RATIO		4
#define DISPLAY_FIT_MODE_XY_RATIO		5

class CLibMilDisp
{
	MIL_ID m_MilDisplay;
	MIL_ID m_MilDisplayOverlay;
	MIL_ID m_MilGraphicContextID;
	long m_lFontName;
	double m_dFontScaleX;
	double m_dFontScaleY;

	long m_lOverlayColor;
	int m_nDisplaySizeX, m_nDisplaySizeY;
	double m_dFitResizeRatio, m_dFitResizeRatioX, m_dFitResizeRatioY;
	int m_nCenterViewOffsetX, m_nCenterViewOffsetY;
	int m_nDisplayFitMode;

	void DisplaySelect(MIL_ID nBufImg, HWND hCtrl);
	
public:
	CLibMilDisp();
	CLibMilDisp(MIL_ID SystemId);
	~CLibMilDisp();

public:
	void CreateOverlay(long nColor);
	void ShowOverlay(CString str);
	void DisplaySelect(MIL_ID nBufImg, HWND hCtrl, CRect rtDispCtrl);
	BOOL ClearOverlay(long pColor);
	BOOL ClearOverlay();
	MIL_ID GetDisplayId();
	MIL_ID GetOverlayId();
	void CalcFitResizeRatio(int nCameraImageSizeX, int nCameraImageSizeY, int nDisplayRectSizeX, int nDisplayRectSizeY, int nDisplayFitMode = DISPLAY_FIT_MODE_CENTERVIEW);
};

