#pragma once

namespace sys
{
//+-----------------------------------------------------------------------------+
//! system tracer																!
//+-----------------------------------------------------------------------------+
struct TraceWin : public SystemTracer
{
public:
	TraceWin(const DWORD&, const _TCHAR*, const _TCHAR*, const _TCHAR*, const uint32_t&);
	~TraceWin();

private:	
	TraceWin() = delete;	
	TraceWin(const TraceWin&) = delete;
	TraceWin& operator=(const TraceWin&) = delete;

public:
	DWORD	m_dwError;
	tstring	m_stText;

public:
	virtual uint32_t Source();	
	virtual tstring Detailed();

private:
	tstring GetCodeString();
};

} // namespace