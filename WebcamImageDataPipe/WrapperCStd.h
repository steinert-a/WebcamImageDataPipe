#pragma once

namespace wrap_cstd
{

uint32_t SetErrno(errno_t);
uint32_t GetErrno(errno_t&);

uint32_t SNPrintF(_TCHAR*, size_t, const _TCHAR*,...);

uint32_t FileOpen(std::FILE**,const _TCHAR*, const _TCHAR*);
uint32_t FileClose(std::FILE**);

uint32_t FileEof(std::FILE*,int&);
uint32_t FileSeek(std::FILE*,long int,int);
uint32_t FileTell(std::FILE*, long int&);

uint32_t FileGetString(_TCHAR*, int, FILE*);
uint32_t FileRead(void*, size_t, size_t, size_t, FILE*, size_t* = NULL);
uint32_t FilePrint(FILE*,const _TCHAR*,...);
uint32_t FileWrite(const void*, size_t, size_t, FILE*, size_t* = NULL);

uint32_t StrNICmp(const _TCHAR*,const _TCHAR*, size_t, int&);
uint32_t StrICmp(const _TCHAR*,const _TCHAR*, int&);

uint32_t StrToInt32(const _TCHAR*,int32_t&,int=10);
uint32_t StrToUInt32(const _TCHAR*,uint32_t&,int=10);
uint32_t StrToFloat(const _TCHAR*,float&);
uint32_t StrToDouble(const _TCHAR*,double&);

}
