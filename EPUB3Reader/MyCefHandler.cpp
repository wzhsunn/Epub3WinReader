#include "stdafx.h"
#include "MyCefHandler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"



//void MyCefHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
//	CEF_REQUIRE_UI_THREAD();
//
//	// Add to the list of existing browsers.
//	browser_list_.push_back(browser);
//}
//
//bool MyCefHandler::DoClose(CefRefPtr<CefBrowser> browser) {
//	CEF_REQUIRE_UI_THREAD();
//
//	// Closing the main window requires special handling. See the DoClose()
//	// documentation in the CEF header for a detailed destription of this
//	// process.
//	if (browser_list_.size() == 1) {
//		// Set a flag to indicate that the window close should be allowed.
//		is_closing_ = true;
//	}
//
//	// Allow the close. For windowed browsers this will result in the OS close
//	// event being sent.
//	return false;
//}
//
//void MyCefHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
//	CEF_REQUIRE_UI_THREAD();
//
//	// Remove from the list of existing browsers.
//	BrowserList::iterator bit = browser_list_.begin();
//	for (; bit != browser_list_.end(); ++bit) {
//		if ((*bit)->IsSame(browser)) {
//			browser_list_.erase(bit);
//			break;
//		}
//	}
//
//	if (browser_list_.empty()) {
//		// All browser windows have closed. Quit the application message loop.
//		CefQuitMessageLoop();
//	}
//}
//
//void MyCefHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
//	CefRefPtr<CefFrame> frame,
//	ErrorCode errorCode,
//	const CefString& errorText,
//	const CefString& failedUrl) {
//	CEF_REQUIRE_UI_THREAD();
//
//	// Don't display an error for downloaded files.
//	if (errorCode == ERR_ABORTED)
//		return;
//
//	// Display a load error message.
//	std::stringstream ss;
//	ss << "<html><body bgcolor=\"white\">"
//		"<h2>Failed to load URL " << std::string(failedUrl) <<
//		" with error " << std::string(errorText) << " (" << errorCode <<
//		").</h2></body></html>";
//	frame->LoadString(ss.str(), failedUrl);
//}
//
//void MyCefHandler::CloseAllBrowsers(bool force_close) {
//	if (!CefCurrentlyOn(TID_UI)) {
//		// Execute on the UI thread.
//		CefPostTask(TID_UI,
//			base::Bind(&MyCefHandler::CloseAllBrowsers, this, force_close));
//		return;
//	}
//
//	if (browser_list_.empty())
//		return;
//
//	BrowserList::const_iterator it = browser_list_.begin();
//	for (; it != browser_list_.end(); ++it)
//		(*it)->GetHost()->CloseBrowser(force_close);
//}


MyCefHandler::MyCefHandler()
{
}

MyCefHandler::~MyCefHandler()
{
}

CefRefPtr<CefBrowser> MyCefHandler::GetBrowser()
{
	base::AutoLock lock_scope(lock_);
	return m_mainBrowser;
}

CefRefPtr<CefLifeSpanHandler> MyCefHandler::GetLifeSpanHandler()
{
	return this;
}


void MyCefHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	base::AutoLock lock_scope(lock_);
	if (!m_mainBrowser.get())
	{
		m_mainBrowser = browser;
	} 
	else
	{
		m_browsers.push_back(browser);
	}
	//CefLifeSpanHandler::OnAfterCreated(browser);
}

void MyCefHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	base::AutoLock lock_scope(lock_);

	if (m_mainBrowser.get() && m_mainBrowser->GetIdentifier() == browser->GetIdentifier())
	{
		//m_mainBrowser = NULL;
		for (auto bit = m_browsers.begin(); bit != m_browsers.end(); ++ bit)
		{
		if ((*bit)->IsLoading())
		{
		(*bit)->StopLoad();
		}
		(*bit)->GetHost()->CloseBrowser(true);
		}
		m_mainBrowser->GetHost()->CloseBrowser(true);
		m_mainBrowser = nullptr;
	}
	else
	{
		m_browsers.erase(std::remove_if(m_browsers.begin(), m_browsers.end(),
			[&](BrowserList::value_type &it){return it->GetIdentifier() == browser->GetIdentifier(); }), m_browsers.end());

	}
}

bool MyCefHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	return false;
}

void MyCefHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	//CEF_REQUIRE_UI_THREAD();
	//base::AutoLock lock_scope(lock_);
	AfxMessageBox(L"load end");
}