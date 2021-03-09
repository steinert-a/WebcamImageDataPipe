#include "PcHeader.h"

#include "Trace.h"
#include "TraceSys.h"
#include "TraceCStd.h"
#include "SystemTrace.h"

#include "WrapperCStd.h"

#include "FilterPipe.h"

using namespace wrap_cstd;

namespace xfp
{
//--------------------------------------------------------------+
// OPTION                                                       !
//--------------------------------------------------------------+
OPTION::OPTION(): m_Name(), m_Value()
{}

OPTION::~OPTION()
{
	clear();
}

OPTION::OPTION(const tstring& p_Name, const tstring& p_Value) : m_Name(p_Name), m_Value(p_Value)
{}

OPTION::OPTION(const OPTION& p_Copy)
{
	(*this) = p_Copy;
}

OPTION& OPTION::operator=(const OPTION& p_Copy)
{
	m_Name = p_Copy.m_Name;
	m_Value = p_Copy.m_Value;

	return (*this);
}

void OPTION::clear()
{
	m_Name.clear();
	m_Value.clear();
}

//--------------------------------------------------------------+
// CONFIG                                                       !
//--------------------------------------------------------------+
CONFIG::CONFIG()
{}

CONFIG::~CONFIG()
{
	clear();
}

CONFIG::CONFIG(const CONFIG& p_Copy)
{
	(*this) = p_Copy;
}


CONFIG& CONFIG::operator=(const CONFIG& p_Copy)
{
	m_Options = p_Copy.m_Options;
	return (*this);
}

void CONFIG::clear()
{
	m_Options.clear();
}

//--------------------------------------------------------------+
// FILTER                                                       !
//--------------------------------------------------------------+
FILTER::FILTER(): m_Type(), m_Id(), m_Flag(0), m_Options()
{}

FILTER::~FILTER()
{
	clear();
}

FILTER::FILTER(const tstring& p_Type, const tstring& p_Id, const uint32_t& p_Flag): 
	m_Type(p_Type), m_Id(p_Id), m_Flag(p_Flag), m_Options()
{}

FILTER::FILTER(const FILTER& p_Copy)
{
	(*this) = p_Copy;
}

FILTER& FILTER::operator=(const FILTER& p_Copy)
{
	m_Type = p_Copy.m_Type;
	m_Id = p_Copy.m_Id;
	m_Flag = p_Copy.m_Flag;
	m_Options = p_Copy.m_Options;

	return (*this);
}

void FILTER::clear()
{
	m_Type.clear();
	m_Id.clear();
	m_Flag = 0;
	m_Options.clear();
}

//--------------------------------------------------------------+
// XFILTERPIPE                                                  !
//--------------------------------------------------------------+
XFILTERPIPE::XFILTERPIPE(): m_Config(), m_Filters()
{}

XFILTERPIPE::XFILTERPIPE(const XFILTERPIPE& p_Copy)
{
	(*this) = p_Copy;
}
		
XFILTERPIPE::~XFILTERPIPE()
{
	clear();
}

XFILTERPIPE& XFILTERPIPE::operator=(const XFILTERPIPE& p_Copy)
{
	m_Config = p_Copy.m_Config;
	m_Filters = p_Copy.m_Filters;

	return (*this);
}

void XFILTERPIPE::clear()
{
	m_Config.clear();
	m_Filters.clear();
}

//--------------------------------------------------------------+
// syncronisation                                               !
//--------------------------------------------------------------+
uint32_t SaveXFilterPipe(const XFILTERPIPE& p_Xml, FILE* p_pFile)
{
	uint32_t nError = 0;
	tstring stDir = _T("");

	if (!nError) nError = ASSERTION_POINTER(p_pFile);

	if (!nError) nError = FilePrint(p_pFile, _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
	if (!nError) nError = FilePrint(p_pFile, _T("<XFILTERPIPE>\n"));
	if (!nError) nError = FilePrint(p_pFile, _T("  <CONFIG>\n"));
	FOR_EACH(itCfg, p_Xml.m_Config.m_Options)
	{
		if (!nError) nError = FilePrint(p_pFile, _T("    <OPTION name = \"%s\" value = \"%s\" />\n"), itCfg->m_Name.c_str(), itCfg->m_Value.c_str());
	}
	if (!nError) nError = FilePrint(p_pFile, _T("  </CONFIG>\n"));
	if (!nError) nError = FilePrint(p_pFile, _T("  <PIPE>\n"));
	FOR_EACH(itFilter, p_Xml.m_Filters)
	{
		if (!nError) nError = FilePrint(p_pFile, _T("    <FILTER type = \"%s\" id = \"%s\" flag = \"%u\" >\n"), itFilter->m_Type.c_str(), itFilter->m_Id.c_str(), itFilter->m_Flag);

		FOR_EACH(itOpt, itFilter->m_Options)
		{
			if (!nError) nError = FilePrint(p_pFile, _T("      <OPTION name = \"%s\" value = \"%s\" />\n"), itOpt->m_Name.c_str(), itOpt->m_Value.c_str());
		}

		if (!nError) nError = FilePrint(p_pFile, _T("    </FILTER>\n"));
	}
	if (!nError) nError = FilePrint(p_pFile, _T("  </PIPE>\n"));
	if (!nError) nError = FilePrint(p_pFile, _T("</XFILTERPIPE>\n"));

	return nError;
}

uint32_t LoadXFilterPipe(QXmlStreamReader* p_pStream, XFILTERPIPE& p_Xml)
{
	uint32_t nError = 0;
	p_Xml.clear();
	bool isPipe = false;
	bool isCfg = false;

	if (!nError) nError = ASSERTION_POINTER(p_pStream);

	while (!nError && !p_pStream->isEndDocument())
	{
		if (p_pStream->isEndElement() && !p_pStream->name().toString().compare(QString("XFILTERPIPE"), Qt::CaseInsensitive))
		{
			break;
		}
		else if (p_pStream->isStartElement() && !p_pStream->name().toString().compare(QString("CONFIG"), Qt::CaseInsensitive))
		{
			isCfg = true;
		}
		else if (p_pStream->isEndElement() && !p_pStream->name().toString().compare(QString("CONFIG"), Qt::CaseInsensitive))
		{
			isCfg = false;
		}
		else if (p_pStream->isStartElement() && !p_pStream->name().toString().compare(QString("PIPE"), Qt::CaseInsensitive))
		{
			isPipe = true;
		}
		else if (p_pStream->isEndElement() && !p_pStream->name().toString().compare(QString("PIPE"), Qt::CaseInsensitive))
		{
			isPipe = false;
		}
		else if (p_pStream->isStartElement() && !p_pStream->name().toString().compare(QString("FILTER"), Qt::CaseInsensitive))
		{
			FILTER filter;
			bool typeExist = false;
			bool idExist = false;

			foreach(const QXmlStreamAttribute& attr, p_pStream->attributes())
			{
				if (attr.name().toString() == QString("type"))
				{
					filter.m_Type = attr.value().toString().toStdWString();
					typeExist = true;
				}
				else if (attr.name().toString() == QString("id"))
				{
					filter.m_Id = attr.value().toString().toStdWString();
					idExist = true;
				}
				else if (attr.name().toString() == QString("flag"))
				{
					nError = wrap_cstd::StrToUInt32(attr.value().toString().toStdWString().c_str(), filter.m_Flag);
				}
			}

			if (typeExist && idExist)
			{
				p_Xml.m_Filters.push_back(filter);
			}
			else
			{
				nError = TRACE_SYS(STE_XML, _T("Filter attribute ist missing!"));
			}
		}
		else if (p_pStream->isStartElement() && !p_pStream->name().toString().compare(QString("OPTION"), Qt::CaseInsensitive))
		{
			OPTION option;

			foreach(const QXmlStreamAttribute& attr, p_pStream->attributes())
			{
				if (nError) break;

				if (attr.name().toString() == QString("name"))
				{
					option.m_Name = attr.value().toString().toStdWString();
				}
				else if (attr.name().toString() == QString("value"))
				{
					option.m_Value = attr.value().toString().toStdWString();
				}
			}

			if (isCfg && !isPipe)
			{
				p_Xml.m_Config.m_Options.push_back(option);
			} 
			else if (isPipe && !isCfg && p_Xml.m_Filters.size())
			{
				p_Xml.m_Filters.back().m_Options.push_back(option);
			}
			else
			{
				nError = TRACE_SYS(STE_XML, _T("Corrupt XML!"));
			}
		}
		else if (p_pStream->hasError())
		{
			QString stError;
			p_pStream->raiseError(stError);
			nError = TRACE_SYS(STE_XML, stError.toStdWString().c_str());
			break;
		}

		p_pStream->readNext();
	}

	if (nError) p_Xml.clear();
	return nError;
}
uint32_t SaveXFilterPipe(const XFILTERPIPE& p_Xml, const tstring& p_FilePath)
{
	uint32_t err = STID_NONE;

	FILE* pFile = NULL;
	if (!err) err = wrap_cstd::FileOpen(&pFile, p_FilePath.c_str(), _T("w"));
	if (!err) err = SaveXFilterPipe(p_Xml, pFile);
	wrap_cstd::FileClose(&pFile);

	return err;
}

uint32_t LoadXFilterPipe(const tstring& p_FilePath, XFILTERPIPE& p_Xml)
{
	uint32_t err = STID_NONE;
	QXmlStreamReader grXmlReader;
	QFile grXmlFile(QString::fromStdWString(p_FilePath));

	if (!err && !grXmlFile.open(QIODevice::ReadOnly)) err = TRACE_SYS(STE_PATH, _T(""));
	if (!err) grXmlReader.setDevice(&grXmlFile);
	if (!err) err = xfp::LoadXFilterPipe(&grXmlReader, p_Xml);

	return err;
}

uint32_t FilterToString(const FILTER& p_Filter, tstring& p_Xml)
{
	uint32_t err = STID_NONE;

	p_Xml.clear();
	wchar_t buffer[1024] = { 0 };
	p_Xml.reserve(2048);
	
	if (!err) err = SNPrintF(buffer, 1024, _T("<FILTER type = \"%s\" id = \"%s\" flag = \"%u\" >\n"), p_Filter.m_Type.c_str(), p_Filter.m_Id.c_str(), p_Filter.m_Flag);
	if (!err) p_Xml += buffer;

	FOR_EACH(itOpt, p_Filter.m_Options)
	{
		if (!err) err = SNPrintF(buffer, 1024, _T("<OPTION name = \"%s\" value = \"%s\" />\n"), itOpt->m_Name.c_str(), itOpt->m_Value.c_str());
		if (!err) p_Xml += buffer;
	}
	if (!err) p_Xml += _T("</FILTER>\n");

	if (err) p_Xml.clear();
	return err;
}

uint32_t FilterFromString(const tstring& p_Xml, FILTER& p_Filter)
{
	uint32_t nError = STID_NONE;
	p_Filter.clear();
	bool exist = false;
	QXmlStreamReader grXmlReader(QString::fromStdWString(p_Xml));

	while (!nError && !grXmlReader.isEndDocument())
	{
		if (grXmlReader.isEndElement() && !grXmlReader.name().toString().compare(QString("FILTER"), Qt::CaseInsensitive))
		{
			break;
		}
		else if (grXmlReader.isStartElement() && !grXmlReader.name().toString().compare(QString("FILTER"), Qt::CaseInsensitive))
		{			
			exist = true;
			bool typeExist = false;
			bool idExist = false;

			foreach(const QXmlStreamAttribute& attr, grXmlReader.attributes())
			{
				if (attr.name().toString() == QString("type"))
				{
					p_Filter.m_Type = attr.value().toString().toStdWString();
					typeExist = true;
				}
				else if (attr.name().toString() == QString("id"))
				{
					p_Filter.m_Id = attr.value().toString().toStdWString();
					idExist = true;
				}
				else if (attr.name().toString() == QString("flag"))
				{
					nError = wrap_cstd::StrToUInt32(attr.value().toString().toStdWString().c_str(), p_Filter.m_Flag);
				}
			}

			if (!typeExist || !idExist)
			{
				nError = TRACE_SYS(STE_XML, _T("Filter attribute ist missing!"));
			}
		}
		else if (grXmlReader.isStartElement() && !grXmlReader.name().toString().compare(QString("OPTION"), Qt::CaseInsensitive))
		{
			OPTION option;

			foreach(const QXmlStreamAttribute& attr, grXmlReader.attributes())
			{
				if (nError) break;

				if (attr.name().toString() == QString("name"))
				{
					option.m_Name = attr.value().toString().toStdWString();
				}
				else if (attr.name().toString() == QString("value"))
				{
					option.m_Value = attr.value().toString().toStdWString();
				}
			}

			p_Filter.m_Options.push_back(option);
		}
		else if (grXmlReader.hasError())
		{
			QString stError;
			grXmlReader.raiseError(stError);
			nError = TRACE_SYS(STE_XML, stError.toStdWString().c_str());
			break;
		}

		grXmlReader.readNext();
	}

	if (!nError && !exist) nError = TRACE_SYS(STE_XML, _T("Filter ist missing!"));
	if (nError) p_Filter.clear();
	return nError;
}

//--------------------------------------------------------------+
// access                                                       !
//--------------------------------------------------------------+
tstring getOption(const std::list<OPTION>& p_Options, const tstring& p_Name, const tstring p_Default)
{
	tstring ret = p_Default;

	FOR_EACH(it, p_Options) 
	{
		if (!it->m_Name.compare(p_Name))
		{
			ret = it->m_Value;
			break;
		}
	}

	return ret;
}

void setOption(std::list<OPTION>& p_Options, const tstring& p_Name, const tstring& p_Value)
{
	bool exist = false;

	FOR_EACH(it, p_Options)
	{
		if (!it->m_Name.compare(p_Name))
		{
			it->m_Value = p_Value;
			exist = true;
			break;
		}
	}

	if (!exist)
	{
		p_Options.push_back(OPTION(p_Name, p_Value));
	}
}

}
