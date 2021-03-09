#pragma once

//+-------------------------------------------------------------+
//! core type definition										!
//+-------------------------------------------------------------+
#ifndef BYTE
#define BYTE unsigned char 
#endif

#ifdef _UNICODE
typedef std::wstring tstring;
#define to_tstring std::to_wstring
#else
typedef std::string tstring;
#define to_tstring std::to_string 
#endif
