#include "stdafx.h"
#include "util.h"
#include <string>
#include <windows.h>

std::wstring Ansi2Wide(const std::string &str)
{
	if (str.length() > 0)
	{
		int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring strTo(size_needed, 0);
		MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &strTo[0], size_needed);
		return strTo;
	}
	else
	{
		return L"";
	}
}