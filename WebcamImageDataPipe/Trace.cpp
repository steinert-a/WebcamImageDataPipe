#include "PcHeader.h"
#include "Trace.h"

using namespace std;

namespace sys
{
//+-----------------------------------------------------------------------------+
//! system tracer global														!
//+-----------------------------------------------------------------------------+
uint32_t g_nSystemTracerId = 1;

//+-----------------------------------------------------------------------------+
//! system tracer																!
//+-----------------------------------------------------------------------------+
SystemTracer::SystemTracer():m_nId(g_nSystemTracerId++)
{
	m_stFile		= _T(""); 
	m_stFunction	= _T(""); 
	m_nLine			= 0;
}

SystemTracer::~SystemTracer()
{}

tstring SystemTracer::GetSourceString()
{
	tstring stSource = _T("");

	switch(Source())
	{
	case STES_SYS:
		stSource = _T("System (SYS)");
		break;
	case STES_MATH:
		stSource = _T("Mathematics");
		break;

	case STES_C_STD:
		stSource = _T("Standard C++ Library");
		break;

	default:
		stSource = _T("Unknown Source");
		break;
	}

	return stSource;
}

uint32_t SystemTracer::Id()
{
	return m_nId;
}

tstring SystemTracer::File()
{
	return m_stFile;
}

tstring SystemTracer::Function()
{
	return m_stFunction;
}

uint32_t SystemTracer::Line()
{
	return m_nLine;
}

} // namespace
