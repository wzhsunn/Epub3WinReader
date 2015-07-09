
// EPUB3ReaderDlg.h : header file
//

#pragma once
#include <map>
#include "CPP2JS.h"
#include "MyCefHandler.h"

// CEPUB3ReaderDlg dialog
#define READER_SCRIPTS_LOCATION   L"\\..\\Scripts"

struct TOCEntry;

class CEPUB3ReaderDlg : public CDialogEx
{
public:
	// Global Readium-shared-JS -> C++ object
	//MyDocHostUIHandler	g_readiumJS2Cpp;
	// Global C++ object -> Readium-shared-JS
	ReadiumJSApi		g_cpp2ReadiumJS;

public:
	static CEPUB3ReaderDlg* pThis;
// Construction
public:
	CEPUB3ReaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EPUB3READER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void SetWindowCaption();
	void digInto(TOCEntry& tocEntry, HTREEITEM hParent);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	//CExplorer m_explorer;
	CefRefPtr<MyCefHandler>		m_cefClient;

	CTreeCtrl m_tree;
	std::map<HTREEITEM, CString> mapHTREEITEM2CString;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnOpenEpub();
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNxt();
	afx_msg void OnReload();
};
