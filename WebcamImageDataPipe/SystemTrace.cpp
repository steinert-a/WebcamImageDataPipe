#include "PcHeader.h"

#include "Trace.h"
#include "SystemTrace.h"

using namespace std;

namespace sys
{
//+-----------------------------------------------------------------------------+
//! system trace global															!
//+-----------------------------------------------------------------------------+
std::atomic<uint32_t> G_ERROR = 0;

//+-----------------------------------------------------------------------------+
//! system trace																!
//+-----------------------------------------------------------------------------+
SYSTEM_TRACE::SYSTEM_TRACE()
{
	// assert default constructor is thread safe
}

SYSTEM_TRACE::~SYSTEM_TRACE()
{
	// assert all threads are done
	m_stlTraceStack.clear();
}

SYSTEM_TRACE* SYSTEM_TRACE::Instance()
{
	static SYSTEM_TRACE grSystemTrace;
	return &grSystemTrace;
}

uint32_t  SYSTEM_TRACE::Trace(shared_ptr<SYSTEM_TRACER> p_pTracer, const bool& p_bThrow)
{
	if(!p_pTracer) return STID_NONE;

	std::lock_guard<std::mutex> grLock(m_grMutex);
	G_ERROR = p_pTracer->Id();

#ifdef _DEBUG
	TRACER(_T("%s\n\n"),p_pTracer->Detailed().c_str());
#endif

	m_stlTraceStack.push_front(p_pTracer);
	if(m_stlTraceStack.size() == SYS_TRACE_STACK_MAX_SIZE)
	{
		m_stlTraceStack.pop_back();
	}

	if(p_bThrow) throw G_ERROR.load();
	return G_ERROR.load();
}

uint32_t SYSTEM_TRACE::LastErrorSource()
{
	std::lock_guard<std::mutex> grLock(m_grMutex);
	uint32_t  nSource = STES_NONE;

	if(m_stlTraceStack.size()) nSource = m_stlTraceStack.front()->Source();

	return nSource;
}

uint32_t SYSTEM_TRACE::LastErrorId()
{
	std::lock_guard<std::mutex> grLock(m_grMutex);
	uint32_t  nId = STID_NONE;

	if(m_stlTraceStack.size()) nId = m_stlTraceStack.front()->Id();

	return nId;
}

bool SYSTEM_TRACE::DetailedErrorId(uint32_t p_nId, tstring& p_nDetails)
{	
	std::lock_guard<std::mutex> grLock(m_grMutex);
	p_nDetails = _T("");

	FOR_EACH(itErr, m_stlTraceStack)
	{
		if((*itErr)->Id() == p_nId) 
		{
			p_nDetails = (*itErr)->Detailed();
			return true;
		}
	}

	return false;
}

} // namespace

//+-----------------------------------------------------------------------------+
//! system trace global															!
//+-----------------------------------------------------------------------------+
void TRACER(const _TCHAR* p_pc, ...)
{
#ifdef _DEBUG
	_TCHAR acBuffer[2048] = {0};
    va_list pArg = NULL;
    va_start(pArg, p_pc);
	_vsntprintf_s(acBuffer,_TRUNCATE,p_pc,pArg);
#ifdef _UNICODE
	_RPTW0(_CRT_WARN, acBuffer);
#else
	_RPT0(_CRT_WARN, acBuffer);
#endif
	va_end(pArg);
#endif
}
