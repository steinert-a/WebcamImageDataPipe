#include "PcHeader.h"

#include "Trace.h"
#include "TraceCStd.h"

using namespace std;

namespace sys
{
//+-----------------------------------------------------------------------------+
//! system tracer C++ standard library											!
//+-----------------------------------------------------------------------------+
TraceCStd::TraceCStd(const errno_t& p_nStc, const _TCHAR* p_pcFile, const _TCHAR *p_pcFunc, const uint32_t& p_nLine)
{	
	m_nErrNo		= p_nStc;
	m_stFile		= p_pcFile;
	m_stFunction	= p_pcFunc;
	m_nLine			= p_nLine;
}

TraceCStd::~TraceCStd()
{}

uint32_t TraceCStd::Source()
{	
	return STES_C_STD;
}

tstring TraceCStd::Detailed()
{
	tstring stDetails	= _T("");
	_TCHAR acBuffer[128] = {0};

	stDetails	+= _T("ROOT: ");
	stDetails	+= to_tstring(Source());
	stDetails	+= _T(" - ");
	stDetails	+= GetSourceString();

	stDetails	+= _T("\nCODE: ");
	stDetails	+= to_tstring(m_nErrNo);

	_tcserror_s(acBuffer,m_nErrNo); // at most 94 characters long.
	if(_tcslen(acBuffer))
	{
		stDetails	+= _T(" - ");
		stDetails	+= acBuffer;
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
