#include "stdafx.h"
#include "LibMil.h"


CLibMil::CLibMil(CWnd* pParent)
{	
	m_pParent = pParent;

	// Allocate defaults.
	MappAllocDefault(M_DEFAULT, &m_MilApplication, &m_MilSystem, &m_MilDisplay, M_NULL, M_NULL);

	m_nDisplaySizeX = 0;
	m_nDisplaySizeY = 0;
}


CLibMil::~CLibMil()
{

	// Free defaults allocations.
	MappFreeDefault(m_MilApplication, m_MilSystem, m_MilDisplay, M_NULL, M_NULL);
}


void CLibMil::Init(CWnd* pParent /*=NULL*/)
{

}

void CLibMil::Copy(MIL_ID nSrc, MIL_ID nDst)
{
	MbufCopy(nSrc, nDst);
}

//void CLibMil::DisplaySelect(CLibMilDisp* pDisplayId, MIL_ID ImageId, HWND hDispCtrl, CRect rtDispCtrl)
//{
//	int nBufferSizeX = 0, nBufferSizeY = 0;
//	nBufferSizeX = MbufInquire(ImageId, M_SIZE_X, M_NULL);
//	nBufferSizeY = MbufInquire(ImageId, M_SIZE_Y, M_NULL);
//
//	m_nDisplaySizeX = rtDispCtrl.Width();
//	m_nDisplaySizeY = rtDispCtrl.Height();
//
//	CalcFitResizeRatio(nBufferSizeX, nBufferSizeY, m_nDisplaySizeX, m_nDisplaySizeY, DISPLAY_FIT_MODE_CENTERVIEW);
//	MdispZoom(m_MilDisplay, m_dFitResizeRatioX, m_dFitResizeRatioY);
//	MdispPan(m_MilDisplay, m_nCenterViewOffsetX, m_nCenterViewOffsetY);
//
//	pDisplayId->DisplaySelect(ImageId, hDispCtrl);
//}

int CLibMil::GetSystemId()
{
	return m_MilSystem;
}

int CLibMil::GetDisplayId()
{
	return m_MilDisplay;
}
