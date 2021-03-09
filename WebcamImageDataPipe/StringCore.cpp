#include "PcHeader.h"

#include "StringCore.h"

namespace core
{
tstring W2T(const std::wstring& p_stWide)
{
#ifdef _UNICODE
	return p_stWide;
#else
	return W2A(p_stWide);
#endif
}

tstring A2T(const std::string& p_stLocal)
{
#ifdef _UNICODE
	return A2W(p_stLocal);
#else
	return p_stLocal;
#endif
}

std::wstring A2W(const std::string& p_stLocal)
{
	std::wstring stWide;
	wchar_t* pWide = NULL;

	size_t nSize = p_stLocal.size()+1;
	pWide = new wchar_t[nSize];
	memset(pWide,0,nSize*sizeof(wchar_t));

	mbstowcs_s(NULL,pWide,nSize,p_stLocal.c_str(),p_stLocal.size());
	stWide = pWide;

	DELETE_ARRAY(pWide);
	return stWide;
}

std::string W2A(const std::wstring& p_stWide)
{
	std::string stLocal;
	char* pLocal = NULL;

	size_t nSize = p_stWide.size()+1;
	pLocal = new char[nSize];
	memset(pLocal,0,nSize*sizeof(char));

	wcstombs_s(NULL,pLocal,nSize,p_stWide.c_str(),p_stWide.size());
	stLocal = pLocal;

	DELETE_ARRAY(pLocal);
	return stLocal;
}

std::string W2U8(const std::wstring& p_stWide)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvtUtf8;
	return cvtUtf8.to_bytes(p_stWide);
}

std::string A2U8(const std::string& p_stLocal)
{
	std::wstring stWide;
	stWide = A2W(p_stLocal);
	return W2U8(stWide);
}

std::string T2U8(const tstring& p_stT)
{
#ifdef _UNICODE
	return W2U8(p_stT);
#else
	return A2U8(p_stT);
#endif
}

std::string U82A(const std::string& p_stUtf8)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvtUtf16;
	return W2A(cvtUtf16.from_bytes(p_stUtf8));
}

std::wstring U82W(const std::string& p_stUtf8)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvtUtf16;
	return cvtUtf16.from_bytes(p_stUtf8);
}

tstring U82T(const std::string& p_stUtf8)
{
#ifdef _UNICODE
	return U82W(p_stUtf8);
#else
	return U82A(p_stUtf8);
#endif
}

std::list<tstring> Split(const tstring& p_stString, const tstring& p_stDelimiter)
{
	std::list<tstring> stlRes;

	std::size_t nPos0 = 0;
	std::size_t nPos1 = p_stString.find_first_of(p_stDelimiter);

	while(nPos1 != std::string::npos)
	{
		stlRes.push_back(p_stString.substr(nPos0,nPos1-nPos0));
		nPos0 = nPos1+1;
		nPos1 = p_stString.find_first_of(p_stDelimiter,nPos0);
	}
	stlRes.push_back(p_stString.substr(nPos0,p_stString.size()-nPos0));

	return stlRes;
}

}