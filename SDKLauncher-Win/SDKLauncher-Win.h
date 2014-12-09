
// SDKLauncherWin.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#define VISUAL_LEAK_DETECTOR 0
#ifdef VISUAL_LEAK_DETECTOR
#include "vld.h"
#endif

#include "resource.h"		// main symbols


// CSDKLauncherWinApp:
// See SDKLauncherWn.cpp for the implementation of this class
//

class CSDKLauncherWinApp : public CWinApp
{
public:
	CSDKLauncherWinApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	
};

extern CSDKLauncherWinApp theApp;
