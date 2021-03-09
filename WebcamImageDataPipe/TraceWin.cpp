#include "PcHeader.h"

#include "Trace.h"
#include "TraceWin.h"

using namespace std;

namespace sys
{

//+-----------------------------------------------------------------------------+
//! system tracer																!
//+-----------------------------------------------------------------------------+
TraceWin::TraceWin(const DWORD &p_dwError, const _TCHAR *p_pcText, const _TCHAR *p_pcFile, const _TCHAR *p_pcFunc, const uint32_t &p_nLine)
{	
	m_dwError		= p_dwError;
	m_stText		= p_pcText;
	m_stFile		= p_pcFile;
	m_stFunction	= p_pcFunc;
	m_nLine			= p_nLine;
}

TraceWin::~TraceWin()
{}

uint32_t TraceWin::Source()
{	
	return STES_WIN;
}

tstring TraceWin::GetCodeString()
{
	tstring stError(_T(""));

	if(m_dwError)
	{
		LPWSTR lpMsgBuf;
		DWORD nBufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			m_dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		if (nBufLen)
		{
			stError	= tstring(lpMsgBuf, lpMsgBuf+nBufLen);     
			LocalFree(lpMsgBuf);

			size_t nPos = stError.find_last_not_of(_T(" \n\r\t"));
			if( string::npos != nPos )
				stError = stError.substr(0,nPos+1);
		}
	}

	return stError;
}

tstring TraceWin::Detailed()
{
	tstring stDetails = _T("");

	stDetails	+= _T("ROOT: ");
	stDetails	+= to_tstring(Source());
	stDetails	+= _T(" - ");
	stDetails	+= GetSourceString();

	stDetails	+= _T("\nCODE: ");
	stDetails	+= to_tstring(m_dwError);
	stDetails	+= _T(" - ");
	stDetails	+= GetCodeString();

	if(m_stText.size() != 0)
	{
		stDetails	+= _T("\nTEXT: ");
		stDetails	+= m_stText;
	}

	stDetails	+= _T("\nFILE: ");
	stDetails	+= m_stFile;

	stDetails	+= _T("\nTASK: ");
	stDetails	+= m_stFunction;

	stDetails	+= _T("\nLINE: ");
	stDetails	+= to_tstring(m_nLine);

	return stDetails;
}

} // namespace