#pragma once

namespace xfp 
{
//--------------------------------------------------------------+
// OPTION                                                       !
//--------------------------------------------------------------+
struct OPTION {
	OPTION();
	OPTION(const tstring&, const tstring&);
	OPTION(const OPTION&);
	~OPTION();
	OPTION& operator=(const OPTION&);

	tstring m_Name;
	tstring m_Value;

	void clear();
};

//--------------------------------------------------------------+
// CONFIG                                                       !
//--------------------------------------------------------------+
struct CONFIG {
	CONFIG();
	CONFIG(const CONFIG&);
	~CONFIG();
	CONFIG& operator=(const CONFIG&);

	std::list<OPTION> m_Options;

	void clear();
};

//--------------------------------------------------------------+
// FILTER                                                       !
//--------------------------------------------------------------+
struct FILTER {
	FILTER();
	FILTER(const tstring&, const tstring&, const uint32_t& = 0);
	FILTER(const FILTER&);
	~FILTER();
	FILTER& operator=(const FILTER&);

	tstring m_Type;
	tstring m_Id;
	uint32_t m_Flag;
	std::list<OPTION> m_Options;

	void clear();
};

//--------------------------------------------------------------+
// XFILTERPIPE                                                  !
//--------------------------------------------------------------+
struct XFILTERPIPE {
	XFILTERPIPE();
	XFILTERPIPE(const XFILTERPIPE&);
	~XFILTERPIPE();
	XFILTERPIPE& operator=(const XFILTERPIPE&);

	CONFIG m_Config;
	std::list<FILTER> m_Filters;

	void clear();
};

//--------------------------------------------------------------+
// syncronisation                                               !
//--------------------------------------------------------------+
uint32_t SaveXFilterPipe(const XFILTERPIPE&, FILE*);
uint32_t LoadXFilterPipe(QXmlStreamReader* ,XFILTERPIPE&);
uint32_t SaveXFilterPipe(const XFILTERPIPE&, const tstring&);
uint32_t LoadXFilterPipe(const tstring&, XFILTERPIPE&);

uint32_t FilterToString(const FILTER&, tstring&);
uint32_t FilterFromString(const tstring&, FILTER&);

//--------------------------------------------------------------+
// access                                                       !
//--------------------------------------------------------------+
tstring getOption(const std::list<OPTION>&, const tstring&, const tstring = _T(""));
void setOption(std::list<OPTION>&, const tstring&, const tstring&);

}
