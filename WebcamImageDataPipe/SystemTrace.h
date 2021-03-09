#pragma once

//+-----------------------------------------------------------------------------+
//! system trace																!
//+-----------------------------------------------------------------------------+
#define SYS_TRACE_STACK_MAX_SIZE		256

namespace sys
{
//+-----------------------------------------------------------------------------+
//! system trace global															!
//+-----------------------------------------------------------------------------+
extern std::atomic<uint32_t> G_ERROR;

//+-----------------------------------------------------------------------------+
//! system trace																!
//+-----------------------------------------------------------------------------+
class SYSTEM_TRACE
{
private:
	SYSTEM_TRACE();
	~SYSTEM_TRACE();	
	SYSTEM_TRACE(const SYSTEM_TRACE&) = delete;
	SYSTEM_TRACE& operator=(const SYSTEM_TRACE&) = delete;

public:
	static SYSTEM_TRACE* Instance();

private:
	std::mutex		m_grMutex;
	STL_TRACER		m_stlTraceStack;

public:
	uint32_t Trace(std::shared_ptr<SYSTEM_TRACER>,const bool&);
	uint32_t LastErrorSource();
	uint32_t LastErrorId();
public:
	bool DetailedErrorId(uint32_t , tstring&);
};

} // namespace

//+-----------------------------------------------------------------------------+
//! system trace global	function												!
//+-----------------------------------------------------------------------------+
void TRACER(const _TCHAR*, ...);	// only for debug window inside Visual Studio

//+-----------------------------------------------------------------------------+
//! system trace global															!
//+-----------------------------------------------------------------------------+
#define SYS_TRACE	sys::SYSTEM_TRACE::Instance()
#define SYS_ERROR	sys::G_ERROR

//+-----------------------------------------------------------------------------+
//! system trace defines														!
//+-----------------------------------------------------------------------------+
#define ASSERTION_POINTER(exp)		((exp)?STES_NONE:sys::SYSTEM_TRACE::Instance()->Trace(std::shared_ptr<SYSTEM_TRACER>(new sys::TraceSys(STE_POINTER,_T("NULL-Pointer"),_T(__FILE__),_T(__FUNCTION__),__LINE__)),false))

#define TRACE_SYS(code,txt)			sys::SYSTEM_TRACE::Instance()->Trace(std::shared_ptr<SYSTEM_TRACER>(new sys::TraceSys(code,txt,_T(__FILE__),_T(__FUNCTION__),__LINE__)),false)
#define THROW_SYS(code,txt)			sys::SYSTEM_TRACE::Instance()->Trace(std::shared_ptr<SYSTEM_TRACER>(new sys::TraceSys(code,txt,_T(__FILE__),_T(__FUNCTION__),__LINE__)),true)
#define THROW_MATH(code,txt)		sys::SYSTEM_TRACE::Instance()->Trace(std::shared_ptr<SYSTEM_TRACER>(new sys::TraceMath(code,txt,_T(__FILE__),_T(__FUNCTION__),__LINE__)),true)
#define TRACE_MATH(code,txt)		sys::SYSTEM_TRACE::Instance()->Trace(std::shared_ptr<SYSTEM_TRACER>(new sys::TraceMath(code,txt,_T(__FILE__),_T(__FUNCTION__),__LINE__)),false)
#define TRACE_WIN(code,txt)			sys::SYSTEM_TRACE::Instance()->Trace(std::shared_ptr<SYSTEM_TRACER>(new sys::TraceWin(code,txt,_T(__FILE__),_T(__FUNCTION__),__LINE__)),false)
#define THROW_WIN(code,txt)			sys::SYSTEM_TRACE::Instance()->Trace(std::shared_ptr<SYSTEM_TRACER>(new sys::TraceWin(code,txt,_T(__FILE__),_T(__FUNCTION__),__LINE__)),true)
#define TRACE_CSTD(en)				sys::SYSTEM_TRACE::Instance()->Trace(std::shared_ptr<SYSTEM_TRACER>(new sys::TraceCStd(en,_T(__FILE__),_T(__FUNCTION__),__LINE__)),false)
