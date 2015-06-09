#include "stdafx.h"
#include "MyCefHandler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

// Custom menu command Ids.
enum client_menu_ids {
	CLIENT_ID_SHOW_DEVTOOLS = MENU_ID_USER_FIRST,
	CLIENT_ID_CLOSE_DEVTOOLS,
	CLIENT_ID_INSPECT_ELEMENT,
	CLIENT_ID_TESTMENU_SUBMENU,
	CLIENT_ID_TESTMENU_CHECKITEM,
	CLIENT_ID_TESTMENU_RADIOITEM1,
	CLIENT_ID_TESTMENU_RADIOITEM2,
	CLIENT_ID_TESTMENU_RADIOITEM3,
};

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


void MyCefHandler::OnBeforeContextMenu(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model) {
	CEF_REQUIRE_UI_THREAD();

	if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
		// Add a separator if the menu already has items.
		if (model->GetCount() > 0)
			model->AddSeparator();

		// Add DevTools items to all context menus.
		model->AddItem(CLIENT_ID_SHOW_DEVTOOLS, "&Show DevTools");
		model->AddItem(CLIENT_ID_CLOSE_DEVTOOLS, "Close DevTools");
		model->AddSeparator();
		model->AddItem(CLIENT_ID_INSPECT_ELEMENT, "Inspect Element");

		// Test context menu features.
		//BuildTestMenu(model);
	}
}

bool MyCefHandler::OnContextMenuCommand(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id,
	EventFlags event_flags) {
	CEF_REQUIRE_UI_THREAD();

	switch (command_id) {
	case CLIENT_ID_SHOW_DEVTOOLS:
		ShowDevTools(browser, CefPoint());
		return true;
	case CLIENT_ID_CLOSE_DEVTOOLS:
		CloseDevTools(browser);
		return true;
	case CLIENT_ID_INSPECT_ELEMENT:
		ShowDevTools(browser, CefPoint(params->GetXCoord(), params->GetYCoord()));
		return true;
	default:  // Allow default handling, if any.
		//return ExecuteTestMenu(command_id);
		return true;
	}
}

void MyCefHandler::ShowDevTools(CefRefPtr<CefBrowser> browser,
	const CefPoint& inspect_element_at) {
	CefWindowInfo windowInfo;
	CefBrowserSettings settings;

#if defined(OS_WIN)
	windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
#endif

	browser->GetHost()->ShowDevTools(windowInfo, this, settings,
		inspect_element_at);
}

inline void MyCefHandler::CloseDevTools(CefRefPtr<CefBrowser> browser) {
	browser->GetHost()->CloseDevTools();
}