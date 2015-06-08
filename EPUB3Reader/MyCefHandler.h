#pragma once

#include "include/cef_client.h"
#include "include/base/cef_lock.h"
#include <list>
class MyCefHandler :
	public CefClient,
	public CefLifeSpanHandler,
	public CefLoadHandler
{

public:

	MyCefHandler();
	virtual	~MyCefHandler();

	// Provide access to the single global instance of this object.
	static MyCefHandler* GetInstance();

	CefRefPtr<CefBrowser> GetBrowser();
#pragma region CefClient  
	// since we are letting the base implementations handle all of the heavy lifting,  
	// these functions just return the this pointer  
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
#pragma endregion // CefClient  

#pragma region CefLifeSpanHandler  
	// cache a reference to the browser  
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	// release the browser reference  
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
#pragma endregion // CefLifeSpanHandler    

#pragma region CefLoadHandler
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{
		return this;
	}
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) OVERRIDE;
#pragma endregion //CefLoadHandler

public:
	typedef std::vector<CefRefPtr<CefBrowser>> BrowserList;
protected:
	// the browser reference  
	CefRefPtr<CefBrowser> m_mainBrowser;
	BrowserList m_browsers;

	// Include the default reference counting implementation.  
	IMPLEMENT_REFCOUNTING(MyCefHandler);
	// Include the default locking implementation.  
	IMPLEMENT_LOCKING(MyCefHandler);
};


