#include "PcHeader.h"

#include "Trace.h"
#include "TraceSys.h"

using namespace std;

namespace sys
{

//+-----------------------------------------------------------------------------+
//! system tracer																!
//+-----------------------------------------------------------------------------+
TraceSys::TraceSys(const uint32_t &p_nStc, const _TCHAR* p_pcText, const _TCHAR* p_pcFile, const _TCHAR* p_pcFunc, const uint32_t &p_nLine)
{	
	m_nCode			= p_nStc;
	m_stText		= p_pcText;
	m_stFile		= p_pcFile;
	m_stFunction	= p_pcFunc;
	m_nLine			= p_nLine;
}

TraceSys::~TraceSys()
{}

uint32_t TraceSys::Source()
{	
	return STES_SYS;
}

tstring TraceSys::GetCodeString()
{	
	tstring stError(_T(""));

	switch(m_nCode)
	{	
	case STE_NONE:
		stError	= _T("Succeed / No Error");
		break;	
	case STE_ASSERT:
		stError	= _T("Assertion Failed");
		break;	

	case STE_POINTER:
		stError	= _T("Unexpected Pointer");
		break;	
	case STE_ARRAY:
		stError	= _T("Unexpected Array");
		break;	
	case STE_ITERATOR:
		stError	= _T("Unexpected Iterator");
		break;	
	case STE_FLAG:
		stError	= _T("Unexpected Flag");
		break;	
	case STE_THREAD:
		stError	= _T("Unexpected Thread Kill");
		break;
	case STE_PATH:
		stError	= _T("Unexpected Path");
		break;
	case STE_TYPE:
		stError	= _T("Unexpected Type");
		break;
	case STE_EXCEPTION:
		stError	= _T("Unexpected Exception");
		break;
	case STE_INDEX:
		stError	= _T("Unexpected Index");
		break;
	case STE_ENUMERATION:
		stError	= _T("Unexpected Enumeration");
		break;
	case STE_PARAMETER:
		stError	= _T("Unexpected Parameter");
		break;
	case STE_STREAM:
		stError	= _T("Unexpected Stream");
		break;
	case STE_SIZE:
		stError	= _T("Unexpected Size");
		break;

	case STE_INI:
		stError	= _T("Input Initialisation File (INI)");
		break;
	case STE_XML:
		stError	= _T("Input Extensible Markup Language (XML)");
		break;
	case STE_CMD:
		stError = _T("Input Command Line (CMD)");
		break;
	case STE_TGA:
		stError	= _T("Input Targa Image File (TGA)");
		break;
	case STE_BMP:
		stError	= _T("Input Windows Bitmap (BMP)");
		break;
	case STE_CF:
		stError	= _T("Input Container File (CF)");
		break;
	case STE_CFG:
		stError	= _T("Input Configuration File (CFG)");
		break;

	case STE_CONVERT:
		stError	= _T("Conversion Failed");
		break;
	case STE_VERSION:
		stError	= _T("Unexpected Version");
		break;
	case STE_PERMISSION:
		stError	= _T("Permission Denied");
		break;
	case STE_INIT:
		stError	= _T("Initialisation Undone");
		break;
	case STE_VALIDATION:
		stError	= _T("Validation Failed");
		break;
	case STE_ACCESS:
		stError	= _T("Access Failed");
		break;

	case STE_PRINTER:
		stError	= _T("Printer");
		break;
	case STE_CAMERA:
		stError	= _T("Camera");
		break;

	case STE_COM:
		stError	= _T("Component Object Model (COM)");
		break;
	case STE_SINGLETON:
		stError	= _T("Singleton Software Pattern");
		break;
	case STE_BRIDGE:
		stError	= _T("Bridge Software Pattern");
		break;

	default:
		stError	= _T("Unknown Error Code");
		break;
	}

	return stError;

}

tstring TraceSys::Detailed()
{
	tstring stDetails = _T("");

	stDetails	+= _T("ROOT: ");
	stDetails	+= to_tstring(Source());
	stDetails	+= _T(" - ");
	stDetails	+= GetSourceString();

	stDetails	+= _T("\nCODE: ");
	stDetails	+= to_tstring(m_nCode);
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