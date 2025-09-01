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

#pragma warning(disable: 4995)

class CLibMil
{
	MIL_ID m_MilApplication;                 /* Application identifier.            */
	MIL_ID m_MilSystem;                      /* System identifier.                 */
	MIL_ID m_MilDisplay;                     /* Display identifier.                */
	MIL_ID m_MilImage;                       /* Image buffer identifier.           */

public:
	CLibMil();
	~CLibMil();

public:
	void Init(CWnd* pParent = NULL);
	void Copy(MIL_ID nSrc, MIL_ID nDst);
	void DisplaySelect(MIL_ID DisplayId, MIL_ID ImageId, HWND hDispCtrl, CRect rtDispCtrl);
};

