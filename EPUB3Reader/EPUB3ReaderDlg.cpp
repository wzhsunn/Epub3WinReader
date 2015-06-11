
// EPUB3ReaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EPUB3Reader.h"
#include "EPUB3ReaderDlg.h"
#include "afxdialogex.h"

#include "util.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEPUB3ReaderDlg dialog


CEPUB3ReaderDlg* CEPUB3ReaderDlg::pThis = nullptr;

CEPUB3ReaderDlg::CEPUB3ReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEPUB3ReaderDlg::IDD, pParent)
{
	pThis = this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//g_cpp2ReadiumJS.WebBrowser = &m_explorer;
}

void CEPUB3ReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EXPLORER1, m_explorer);
	DDX_Control(pDX, IDC_TREE_TOC, m_tree);
}

BEGIN_MESSAGE_MAP(CEPUB3ReaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_OPEN_EPUB, &CEPUB3ReaderDlg::OnOpenEpub)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TOC, &CEPUB3ReaderDlg::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CEPUB3ReaderDlg::OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NXT, &CEPUB3ReaderDlg::OnBnClickedButtonNxt)
	ON_COMMAND(ID_RELOAD, &CEPUB3ReaderDlg::OnReload)
END_MESSAGE_MAP()


// CEPUB3ReaderDlg message handlers

BOOL CEPUB3ReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	g_cpp2ReadiumJS.initReadiumSDK();

	//this->OnOpenEpub();

	m_cefClient = new MyCefHandler();
	CefWindowInfo info;
	CRect	rc/*(0, 0, 800, 600)*/;
	this->GetClientRect(rc);
	CRect rcTree;
	m_tree.GetClientRect(rcTree);
	rc.SetRect(rcTree.right + 10, rc.top + 5, rc.right - 10, rc.bottom - 5);
	rc.MoveToX(rcTree.right + 10);
	
	info.SetAsChild(m_hWnd, rc);
	std::string strUIPath =
		"http://localhost:5000/reader.html";
	BOOL bRet = CefBrowserHost::CreateBrowser(
		info,
		static_cast<CefRefPtr<CefClient>>(m_cefClient),
		strUIPath,
		CefBrowserSettings(),
		NULL
		);

	g_cpp2ReadiumJS.m_handler = m_cefClient;

	


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEPUB3ReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEPUB3ReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEPUB3ReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEPUB3ReaderDlg::SetWindowCaption()
{
	std::string sTitle = g_cpp2ReadiumJS.getBookTitle();
	std::wstring wstr = Ansi2Wide(sTitle);
	sTitle = Wide2Ansi(wstr, CP_ACP);
	CString cstr(sTitle.c_str());
	if (!cstr.IsEmpty())
	{
		cstr += CString("-EPUB3Reader[EPUB3ÔÄ¶ÁÆ÷]");
	}
	else
	{
		cstr = CString("EPUB3Reader[EPUB3ÔÄ¶ÁÆ÷]");
	}
	SetWindowText((LPCTSTR)cstr);
}


void CEPUB3ReaderDlg::digInto(TOCEntry& tocEntry, HTREEITEM hParent)
{
	for (int i = 0; i < (int)tocEntry.arrChildren.size(); ++i)
	{
		//ÂÒÂë½â¾ö
		std::string tocName = tocEntry.arrChildren[i].sTOCName;
		std::wstring wstr = Ansi2Wide(tocName);
		tocName = Wide2Ansi(wstr, CP_ACP);
		tocEntry.arrChildren[i].sTOCName = tocName;

		CString ss(tocEntry.arrChildren[i].sTOCName.c_str());

		//LPTSTR s1 = ss.GetBuffer();
		//std::string ss2 = utf8_encode(ss.GetBuffer());
		//std::wstring ss3 = s2ws(ss2);
		//TRACE("\n");
		//TRACE(ss3.c_str());
		//tocEntry.arrChildren[i].sTOCName = utf8_encode(ss3);
		//ss = tocEntry.arrChildren[i].sTOCName.c_str();
		//TRACE("\n");
		CString ss1(tocEntry.arrChildren[i].sURI.c_str());

		HTREEITEM hItem = 0;
		if (hParent != 0)
		{
			hItem = m_tree.InsertItem(ss, hParent);
		}
		else
		{
			hItem = m_tree.InsertItem(ss);
		}
		mapHTREEITEM2CString[hItem] = ss1;
		if (tocEntry.arrChildren[i].arrChildren.size())
		{
			digInto(tocEntry.arrChildren[i], hItem);
		}
	}
}

std::map<std::string, std::string>	_fileExtension2MimeMap;
std::string getMime(CString sFilename)
{
	if (_fileExtension2MimeMap.size() == 0)
	{
		_fileExtension2MimeMap["css"] = "text/css";
		_fileExtension2MimeMap["htm"] = "text/html";
		_fileExtension2MimeMap["html"] = "text/html";
		_fileExtension2MimeMap["xml"] = "text/xml";
		_fileExtension2MimeMap["java"] = "text/x-java-source, text/java";
		_fileExtension2MimeMap["txt"] = "text/plain";
		_fileExtension2MimeMap["asc"] = ("text/plain");
		_fileExtension2MimeMap["gif"] = ("image/gif");
		_fileExtension2MimeMap["jpg"] = ("image/jpeg");
		_fileExtension2MimeMap["jpeg"] = ("image/jpeg");
		_fileExtension2MimeMap["png"] = ("image/png");
		_fileExtension2MimeMap["mp3"] = ("audio/mpeg");
		_fileExtension2MimeMap["m3u"] = ("audio/mpeg-url");
		_fileExtension2MimeMap["mp4"] = ("video/mp4");
		_fileExtension2MimeMap["ogv"] = ("video/ogg");
		_fileExtension2MimeMap["flv"] = ("video/x-flv");
		_fileExtension2MimeMap["mov"] = ("video/quicktime");
		_fileExtension2MimeMap["swf"] = ("application/x-shockwave-flash");
		_fileExtension2MimeMap["js"] = ("application/javascript");
		_fileExtension2MimeMap["pdf"] = ("application/pdf");
		_fileExtension2MimeMap["doc"] = ("application/msword");
		_fileExtension2MimeMap["ogg"] = ("application/x-ogg");
		_fileExtension2MimeMap["zip"] = ("application/octet-stream");
		_fileExtension2MimeMap["exe"] = ("application/octet-stream");
		_fileExtension2MimeMap["class"] = ("application/octet-stream");
		_fileExtension2MimeMap["webm"] = ("video/webm");
		_fileExtension2MimeMap["otf"] = "font/opentype";
		_fileExtension2MimeMap["xhtml"] = "application/xhtml+xml";
	}
	CString folderPath;
	int pos = sFilename.ReverseFind('.');
	if (pos != -1)
	{
		folderPath = sFilename.Mid(pos + 1);
	}

	std::string strStd = CT2CA(folderPath);

	//std::string sResult = _fileExtension2MimeMap[strStd];

	std::map<std::string, std::string>::iterator it = _fileExtension2MimeMap.find(strStd);
	if (it != _fileExtension2MimeMap.end())
		return (it->second);

	return "";
}

CString getWebServerPath()
{
	TCHAR szDirectory[MAX_PATH];
	::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory);

	CString sDirectoryPath(szDirectory);

	sDirectoryPath += READER_SCRIPTS_LOCATION;

	return sDirectoryPath;
}

bool getResponseStringAndMime(PCSTR rawURL, BYTE** bytes, ULONGLONG* pSize, std::string& mimeTxt)
{
	CString path = getWebServerPath();
	CString filePath = path + rawURL;
	CString pureURI(rawURL);

	mimeTxt = getMime(pureURI);

	// TRACE("Filepath: %s\n", CT2A(filePath));

	CFile  fp1;
	CFileStatus status;
	if (fp1.GetStatus(filePath, status) == TRUE)
	{
		if (fp1.Open(filePath, CFile::modeRead | CFile::typeBinary))
		{
			ULONGLONG dwLength = fp1.GetLength();
			*pSize = dwLength;
			// make room for whole file, plus null
			BYTE *buffer = new BYTE[(unsigned int)dwLength];	// TODO:  This limits the buffer size to 4 GB (!)
			memset(buffer, 0, (unsigned int)dwLength);
			fp1.Read(buffer, (unsigned int)dwLength); // read whole file
			*bytes = buffer;
			//delete [] buffer;
			fp1.Close();
			return true;
		}
	}

	// don't try to open trivial URLs.  The browser will sometimes request '/', which will cause the lower levels to try and 
	// open it, which generates a range error.
	return (CString::StringLength(pureURI) <= 1) ? false : CEPUB3ReaderDlg::pThis->g_cpp2ReadiumJS.getByteResp(std::string(CT2CA(pureURI)), bytes, pSize);
}
#define  EPUB_PATH "G:\\data\\alice3.epub"
void CEPUB3ReaderDlg::OnOpenEpub()
{
	// TODO: Add your command handler code here

	CFileDialog fileDialog(TRUE, NULL, L"*.epub");
	int result = fileDialog.DoModal();
	if (result == IDOK)
	{
		//AfxMessageBox(fileDialog.GetPathName());

		CT2CA pszConvertedAnsiString(fileDialog.GetPathName());
		std::string path(pszConvertedAnsiString);
		//std::string path(EPUB_PATH);
		TRACE(path.c_str());
		try{
			g_cpp2ReadiumJS.on_actionOpen_ePub3(path);
		}
		catch (std::exception& rEx)
		{
			std::string sErrorString(rEx.what());
			CString ss(CA2T(sErrorString.c_str()));

			AfxMessageBox(ss);
			return;
		}
		catch (...)
		{
			AfxMessageBox((LPCTSTR)"Unknown Exception!");
			return;
		}

		SetWindowCaption();
		std::list<std::pair<std::string, std::string> > spineList = g_cpp2ReadiumJS.getSpineList();
		// fill up the tree control
		m_tree.DeleteAllItems();
		mapHTREEITEM2CString.clear();
		/*std::list<std::pair<std::string, std::string> >::const_iterator iter = spineList.begin();
		for (; iter != spineList.end(); ++iter)
		{
		std::string s = iter->first;
		std::string s1 = iter->second;
		CString ss(s.c_str());
		CString ss1(s1.c_str());
		HTREEITEM hItem = m_tree.InsertItem(ss);
		mapHTREEITEM2CString[hItem] = ss1;

		}*/

		TOCEntry tocEntry;
		g_cpp2ReadiumJS.getTOCList(tocEntry);
		m_tree.DeleteAllItems();
		digInto(tocEntry, 0);

		HTREEITEM item = m_tree.GetFirstVisibleItem();
		m_tree.SelectItem(item);
	}
}

void CEPUB3ReaderDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//CString s = m_tree.GetItemText(pNMTreeView->itemNew.hItem);
	HTREEITEM item = pNMTreeView->itemNew.hItem;
	CString s = mapHTREEITEM2CString[item];
	g_cpp2ReadiumJS.turnMediaOverlayOff();
	g_cpp2ReadiumJS.openContentUrl(std::string(CT2CA(s)), "");
	//AfxMessageBox(s);
	//g_cpp2ReadiumJS.openContentUrl("ai2d_d1e1002", "content1.xhtml");
	//g_cpp2ReadiumJS.openContentUrl("chapter_003.xhtml", "");
	//g_cpp2ReadiumJS.openContentUrl("file01.xhtml", "toch_0004");
	*pResult = 0;
}

void CEPUB3ReaderDlg::OnBnClickedButtonPre()
{
	// TODO: Add your control notification handler code here
	g_cpp2ReadiumJS.turnMediaOverlayOff();
	g_cpp2ReadiumJS.openPageLeft();
}


void CEPUB3ReaderDlg::OnBnClickedButtonNxt()
{
	// TODO: Add your control notification handler code here
	g_cpp2ReadiumJS.turnMediaOverlayOff();
	g_cpp2ReadiumJS.openPageRight();
}


void CEPUB3ReaderDlg::OnReload()
{
	// TODO: Add your command handler code here
	CefRefPtr<CefBrowser> browser;
	if (m_cefClient.get())
		browser = m_cefClient->GetBrowser();
	if (browser)
	{
		browser->Reload();
	}
}
