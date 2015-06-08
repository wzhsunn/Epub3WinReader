#pragma once
#include "include/cef_app.h"  
class MyCefApp :
	public CefApp
{
public:
	MyCefApp();
	virtual ~MyCefApp();

private:
	IMPLEMENT_REFCOUNTING(MyCefApp);
};

