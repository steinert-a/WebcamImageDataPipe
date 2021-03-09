#pragma once

namespace sys
{
//+-----------------------------------------------------------------------------+
//! system tracer C++ standard library											!
//+-----------------------------------------------------------------------------+
struct TraceCStd : public SystemTracer
{
public:
	TraceCStd(const errno_t&, const _TCHAR*, const _TCHAR*, const uint32_t&);
	~TraceCStd();

private:	
	TraceCStd() = delete;	
	TraceCStd(const TraceCStd&) = delete;
	TraceCStd& operator=(const TraceCStd&) = delete;

public:
	errno_t		m_nErrNo;

public:
	virtual uint32_t Source();	
	virtual tstring Detailed();
};

}