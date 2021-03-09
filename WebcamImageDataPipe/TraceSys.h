#pragma once

namespace sys
{
//+-----------------------------------------------------------------------------+
//! system tracer defines														!
//+-----------------------------------------------------------------------------+
#define STE_NONE		0
#define STE_ASSERT		1

// unexpected
#define STE_POINTER		1000
#define STE_ARRAY		1001
#define STE_ITERATOR	1002
#define STE_FLAG		1003
#define STE_THREAD		1004
#define STE_PATH		1005
#define STE_TYPE		1006
#define STE_EXCEPTION	1007
#define STE_INDEX		1008
#define STE_ENUMERATION	1009
#define STE_PARAMETER	1010
#define STE_STREAM		1011
#define STE_SIZE		1012

// user input
#define STE_INI			2001
#define STE_XML			2002
#define STE_CMD			2003
#define STE_TGA			2006
#define STE_BMP			2007
#define STE_CF			2008
#define STE_CFG			2009

// procedure
#define STE_PERMISSION	3000
#define STE_INIT		3001
#define STE_CONVERT		3002
#define STE_VERSION		3003
#define STE_VALIDATION	3004
#define STE_ACCESS		3005

// device
#define STE_PRINTER		4000
#define STE_CAMERA		4001

// pattern 
#define STE_COM			5000
#define STE_SINGLETON	5001
#define STE_BRIDGE		5002

//+-----------------------------------------------------------------------------+
//! system tracer																!
//+-----------------------------------------------------------------------------+
struct TraceSys : public SystemTracer
{
public:
	TraceSys(const uint32_t&, const _TCHAR*, const _TCHAR*, const _TCHAR*, const uint32_t&);
	~TraceSys();

private:	
	TraceSys() = delete;	
	TraceSys(const TraceSys&) = delete;
	TraceSys& operator=(const TraceSys&) = delete;

private:
	uint32_t	m_nCode;
	tstring		m_stText;

public:
	virtual uint32_t Source();	
	virtual tstring Detailed();

private:
	tstring GetCodeString();
};

} // namespace