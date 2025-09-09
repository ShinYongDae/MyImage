#include "stdafx.h"
#include "LibMilDisp.h"


CLibMilDisp::CLibMilDisp(MIL_ID SystemId)
{
	m_MilDisplay = M_NULL;
	MdispAlloc(SystemId, M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &m_MilDisplay);

	m_MilDisplayOverlay = M_NULL;
	m_lOverlayColor = 0;
	m_nDisplaySizeX = 0;
	m_nDisplaySizeY = 0;
	m_dFitResizeRatio = 1.0;
	m_dFitResizeRatioX = 1.0;
	m_dFitResizeRatioY = 1.0;
	m_nCenterViewOffsetX = 0;
	m_nCenterViewOffsetY = 0;
	m_nDisplayFitMode = DISPLAY_FIT_MODE_CENTERVIEW;
}

CLibMilDisp::CLibMilDisp()
{
}

CLibMilDisp::~CLibMilDisp()
{
	// Draw
	//if (m_pMilOvrCad[nIdx])
	//{
	//	delete m_pMilOvrCad[nIdx];
	//	m_pMilOvrCad[nIdx] = NULL;
	//}

	//if (m_pMilDelOvrCad[nIdx])
	//{
	//	delete m_pMilDelOvrCad[nIdx];
	//	m_pMilDelOvrCad[nIdx] = NULL;
	//}

	if (m_MilDisplay)
	{
		MdispFree(m_MilDisplay);
		m_MilDisplay = NULL;
	}
}

void CLibMilDisp::DisplaySelect(MIL_ID nBufImg, HWND hDispCtrl, CRect rtDispCtrl)
{
	int nBufferSizeX = 0, nBufferSizeY = 0;
	nBufferSizeX = MbufInquire(nBufImg, M_SIZE_X, M_NULL);
	nBufferSizeY = MbufInquire(nBufImg, M_SIZE_Y, M_NULL);

	m_nDisplaySizeX = rtDispCtrl.Width();
	m_nDisplaySizeY = rtDispCtrl.Height();

	CalcFitResizeRatio(nBufferSizeX, nBufferSizeY, m_nDisplaySizeX, m_nDisplaySizeY, DISPLAY_FIT_MODE_CENTERVIEW);
	MdispZoom(m_MilDisplay, m_dFitResizeRatioX, m_dFitResizeRatioY);
	MdispPan(m_MilDisplay, m_nCenterViewOffsetX, m_nCenterViewOffsetY);

	DisplaySelect(nBufImg, hDispCtrl);
}

void CLibMilDisp::DisplaySelect(MIL_ID nBufImg, HWND hCtrl)
{
	MdispSelectWindow(m_MilDisplay, nBufImg, hCtrl);
}

void CLibMilDisp::CalcFitResizeRatio(int nCameraImageSizeX, int nCameraImageSizeY, int nDisplayRectSizeX, int nDisplayRectSizeY, int nDisplayFitMode)
{
	m_nDisplayFitMode = nDisplayFitMode;
	m_dFitResizeRatioX = (float)nDisplayRectSizeX / (float)nCameraImageSizeX;
	m_dFitResizeRatioY = (float)nDisplayRectSizeY / (float)nCameraImageSizeY;

	switch (nDisplayFitMode)
	{
	case DISPLAY_FIT_MODE_CENTERVIEW:
		m_dFitResizeRatio = 1.0;
		break;
	case DISPLAY_FIT_MODE_MIN_RATIO:
		m_dFitResizeRatio = min(m_dFitResizeRatioX, m_dFitResizeRatioY);
		break;
	case DISPLAY_FIT_MODE_MAX_RATIO:
		m_dFitResizeRatio = max(m_dFitResizeRatioX, m_dFitResizeRatioY);
		break;
	case DISPLAY_FIT_MODE_X_RATIO:
		m_dFitResizeRatio = m_dFitResizeRatioX;
		break;
	case DISPLAY_FIT_MODE_Y_RATIO:
		m_dFitResizeRatio = m_dFitResizeRatioY;
		break;
	case DISPLAY_FIT_MODE_XY_RATIO:
		break;
	}

	if (nDisplayFitMode != DISPLAY_FIT_MODE_XY_RATIO)
	{
		m_dFitResizeRatioX = m_dFitResizeRatio;
		m_dFitResizeRatioY = m_dFitResizeRatio;

		m_nCenterViewOffsetX = (int)(((m_dFitResizeRatioX*(float)nCameraImageSizeX) - nDisplayRectSizeX) / 2 / m_dFitResizeRatioX);
		m_nCenterViewOffsetY = (int)(((m_dFitResizeRatioY*(float)nCameraImageSizeY) - nDisplayRectSizeY) / 2 / m_dFitResizeRatioY);
	}
	else
	{
		m_nCenterViewOffsetX = 0;
		m_nCenterViewOffsetY = 0;
	}
}

BOOL CLibMilDisp::ClearOverlay(long pColor)
{
	if (m_MilDisplayOverlay)
	{
		MbufClear(m_MilDisplayOverlay, (double)pColor);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLibMilDisp::ClearOverlay()
{
	if (m_MilDisplayOverlay)
	{
		MbufClear(m_MilDisplayOverlay, (double)m_lOverlayColor);
		return TRUE;
	}
	else
		return FALSE;
}

MIL_ID CLibMilDisp::GetDisplayId()
{
	return m_MilDisplay;
}

MIL_ID CLibMilDisp::GetOverlayId()
{
	return m_MilDisplayOverlay;
}

void CLibMilDisp::CreateOverlay(long nColor)
{
	MdispControl(m_MilDisplay, M_OVERLAY, M_ENABLE);
	MdispInquire(m_MilDisplay, M_OVERLAY_ID, &m_MilDisplayOverlay);

	// Clear the overlay to transparent.
	MdispControl(m_MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
	MdispControl(m_MilDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	nColor = MdispInquire(m_MilDisplay, M_TRANSPARENT_COLOR, M_NULL); 
	MbufClear(m_MilDisplayOverlay, (double)nColor);

	m_lOverlayColor = nColor;
/*
	// Draw
	if (m_pMilOvrCad[nIdx])
	{
		delete m_pMilOvrCad[nIdx];
		m_pMilOvrCad[nIdx] = NULL;
	}

	if (!m_pMilOvrCad[nIdx])
	{
		m_pMilOvrCad[nIdx] = m_pMil->AllocDraw(m_pMilDispCad[nIdx]);
		m_pMilOvrCad[nIdx]->SetDrawColor(M_COLOR_RED);
		m_pMilOvrCad[nIdx]->SetDrawBackColor(m_pMilDispCad[nIdx]->m_lOverlayColor);
	}

	if (m_pMilDelOvrCad[nIdx])
	{
		delete m_pMilDelOvrCad[nIdx];
		m_pMilDelOvrCad[nIdx] = NULL;
	}

	if (!m_pMilDelOvrCad[nIdx])
	{
		m_pMilDelOvrCad[nIdx] = m_pMil->AllocDraw(m_pMilDispCad[nIdx]);
		m_pMilDelOvrCad[nIdx]->SetDrawColor(m_pMilDispCad[nIdx]->m_lOverlayColor);
		m_pMilDelOvrCad[nIdx]->SetDrawBackColor(m_pMilDispCad[nIdx]->m_lOverlayColor);
	}
*/
}
