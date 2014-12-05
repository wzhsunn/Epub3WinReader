#pragma once
// from HTTP Server Sample Application
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa364640(v=vs.85).aspx
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <http.h>
#include <stdio.h>

#pragma comment(lib, "httpapi.lib")


void startMsHTTPServerThread();