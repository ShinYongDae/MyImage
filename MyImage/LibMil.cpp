#include "stdafx.h"
#include "LibMil.h"


CLibMil::CLibMil()
{	
	// Allocate defaults.
	MappAllocDefault(M_DEFAULT, &m_MilApplication, &m_MilSystem, &m_MilDisplay, M_NULL, M_NULL);
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

void CLibMil::DisplaySelect(MIL_ID DisplayId, MIL_ID ImageId, HWND hDispCtrl, CRect rtDispCtrl)
{

}