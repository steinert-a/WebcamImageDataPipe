#pragma once

namespace core
{
std::wstring A2W(const std::string&);
tstring A2T(const std::string&);

std::string W2A(const std::wstring&);
tstring W2T(const std::wstring&);

std::string U82A(const std::string&);
std::wstring U82W(const std::string&);
tstring U82T(const std::string&);

std::string A2U8(const std::string&);
std::string W2U8(const std::wstring&);
std::string T2U8(const tstring&);

std::list<tstring> Split(const tstring&, const tstring&);
}