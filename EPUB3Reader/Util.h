#pragma once

//codePage CP_ACP
std::wstring Ansi2Wide(const std::string &str, UINT codePage = CP_UTF8);
std::string Wide2Ansi(const std::wstring &wstr, UINT codePage = CP_UTF8);
