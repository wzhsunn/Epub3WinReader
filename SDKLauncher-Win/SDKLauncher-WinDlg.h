
// SDKLauncherWinDlg.h : header file
//

#pragma once
#include "explorer.h"
#include "afxcmn.h"
#include "WndResizer.h"
#include <map>


// CSDKLauncherWinDlg dialog
struct TOCEntry;

class CSDKLauncherWinDlg : public CDialogEx
{
// Construction
public:
	CSDKLauncherWinDlg(CWnd* pParent = NULL);	// standard constructor
	
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
