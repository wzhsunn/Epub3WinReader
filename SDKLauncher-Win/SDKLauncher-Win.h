// 
//  SDKLauncherWin.h : main header file for the PROJECT_NAME application
//
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification,
//  are permitted provided that the following conditions are met :
//  1. Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation and / or
//  other materials provided with the distribution.
//  3. Neither the name of the organization nor the names of its contributors may
//  be used to endorse or promote products derived from this software without
//  specific prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
//  THE POSSIBILITY OF SUCH DAMAGE.#include "stdafx.h"
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

//#define VISUAL_LEAK_DETECTOR 
#ifdef VISUAL_LEAK_DETECTOR
#include "vld.h"
#endif

#include "resource.h"		// main symbols


// CSDKLauncherWinApp:
// See SDKLauncherWn.cpp for the implementation of this class
//
#include "MsHttpServer.h"
class CSDKLauncherWinApp : public CWinApp
{
	MSHttpAPIWrapper httpServer;
public:
	CSDKLauncherWinApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	
};

extern CSDKLauncherWinApp theApp;

struct _DEBUG_STATE
{
	_CrtMemState state;

	_DEBUG_STATE() {}
	~_DEBUG_STATE()
	{
		reportLeaks();
		TerminateProcess(::GetCurrentProcess(), 0);
		//_CrtDumpMemoryLeaks(); 
		//_CrtSetDbgFlag(0); 
		//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	}
	static _DEBUG_STATE ds;
	static void setMemCheckPoint()
	{
		_CrtMemCheckpoint(&ds.state);
	}
	void reportLeaks()
	{
		_CrtMemDumpAllObjectsSince(&state);
	}
};