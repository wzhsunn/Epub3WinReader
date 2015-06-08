
// EPUB3Reader.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "MsHttpServer.h"


// CEPUB3ReaderApp:
// See EPUB3Reader.cpp for the implementation of this class
//

class CEPUB3ReaderApp : public CWinApp
{
	MSHttpAPIWrapper httpServer;

public:
	CEPUB3ReaderApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEPUB3ReaderApp theApp;