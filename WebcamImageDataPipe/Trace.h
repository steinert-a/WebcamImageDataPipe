#pragma once

//+-----------------------------------------------------------------------------+
//! system tracer defines														!
//+-----------------------------------------------------------------------------+
// system trace id
#define STID_NONE		0

// system trace error source
#define STES_NONE		0

#define STES_SYS		100
#define STES_MATH		101

#define STES_C_STD		200
#define STES_WIN		201

namespace sys
{
//+-----------------------------------------------------------------------------+
//! system tracer global														!
//+-----------------------------------------------------------------------------+
extern uint32_t g_nSystemTracerId;

//+-----------------------------------------------------------------------------+
//! system tracer																!
//+-----------------------------------------------------------------------------+
struct SystemTracer
{
public:
	SystemTracer();
	~SystemTracer();

private:
	SystemTracer(const SystemTracer&);
	SystemTracer& operator=(const SystemTracer&);

private:
	const uint32_t	m_nId;
protected:
	tstring			m_stFile; 
	tstring			m_stFunction; 
	uint32_t		m_nLine;

public:
	virtual uint32_t Source() = 0;	
	virtual tstring Detailed() = 0;

public:
	uint32_t Id();
	tstring File();
	tstring Function();
	uint32_t Line();
	tstring GetSourceString();
};

} // namespace

typedef sys::SystemTracer							SYSTEM_TRACER;
typedef std::list<std::shared_ptr<SYSTEM_TRACER>>	STL_TRACER;
