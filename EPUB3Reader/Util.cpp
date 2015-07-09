#include "stdafx.h"
#include "util.h"
#include <string>
#include <windows.h>

std::wstring Ansi2Wide(const std::string &str, UINT codePage)
{
	if (str.length() > 0)
	{
		int size_needed = MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring strTo(size_needed, 0);
		MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), &strTo[0], size_needed);
		return strTo;
	}
	else
	{
		return L"";
	}
}

std::string Wide2Ansi(const std::wstring &wstr, UINT codePage)
{
	if (wstr.length() > 0)
	{
		int size_needed = WideCharToMultiByte(codePage, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(codePage, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}
	else
	{
		return "";
	}
}
