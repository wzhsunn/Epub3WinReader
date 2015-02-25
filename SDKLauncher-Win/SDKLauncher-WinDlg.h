//
//  SDKLauncherWinDlg.h : header file
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
#include "explorer.h"
#include "afxcmn.h"
#include "WndResizer.h"
#include <map>

#include "JS2CPP.h"
#include "CPP2JS.h"

// CSDKLauncherWinDlg dialog
struct TOCEntry;

#define READER_SCRIPTS_LOCATION   L"\\..\\Scripts"
class CSDKLauncherWinDlg : public CDialogEx
{
public:	
	// Global Readium-shared-JS -> C++ object
	MyDocHostUIHandler	g_readiumJS2Cpp;
	// Global C++ object -> Readium-shared-JS
	ReadiumJSApi		g_cpp2ReadiumJS;

	
	// Construction
public:
	static CSDKLauncherWinDlg* pThis;

	CSDKLauncherWinDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSDKLauncherWinDlg()
	{
	}
	
	void SetWindowCaption();
	void digInto(TOCEntry& tocEntry, HTREEITEM hParent);

// Dialog Data
	enum { IDD = IDD_SDKLAUNCHER_WIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CWndResizer m_resizer;
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Trident Browser control
	CExplorer m_explorer;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	DECLARE_EVENTSINK_MAP()
	void DownloadCompleteExplorer1();
	afx_msg void OnFileOpenepub3file();
	// TOC or Spine
	CTreeCtrl m_tree;
	std::map<HTREEITEM, CString> mapHTREEITEM2CString;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
