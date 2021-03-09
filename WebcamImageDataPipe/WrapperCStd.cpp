#include "PcHeader.h"

#include "Trace.h"
#include "TraceSys.h"
#include "TraceCStd.h"

#include "SystemTrace.h"

#include "WrapperCStd.h"


namespace wrap_cstd
{

uint32_t SetErrno(errno_t p_nNewCStdErr)
{
	uint32_t nError = 0;
	errno_t nCStdErr = 0;

	if(!nError) nCStdErr = _set_errno(p_nNewCStdErr);
	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);	

	return nError;
}

uint32_t GetErrno(errno_t& p_nCStdErr)
{
	uint32_t nError = 0;
	errno_t nCStdErr = 0;

	p_nCStdErr = 0;

	if(!nError) nCStdErr = _get_errno(&p_nCStdErr);
	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);	

	return nError;
}

uint32_t SNPrintF(_TCHAR* p_pBuffer, size_t p_nBufferSize, const _TCHAR* p_pcFormat, ...)
{
	uint32_t nError = 0;
	int nRes = 0;
	va_list pArg = NULL;
	errno_t nCStdErr = 0;

	if (!nError) nError = ASSERTION_POINTER(p_pBuffer);
	if (!nError) nError = ASSERTION_POINTER(p_pcFormat);

	if (!nError) nError = SetErrno(nCStdErr);
	if (!nError)
	{
		va_start(pArg, p_pcFormat);
		nRes = _vsntprintf(p_pBuffer, p_nBufferSize, p_pcFormat, pArg);
		va_end(pArg);
	}
	if (!nError) nError = GetErrno(nCStdErr);

	if (!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);
	if (!nError && nRes < 0) nError = TRACE_CSTD(ENOMSG);

	return nError;
}

uint32_t FileOpen(std::FILE** p_ppFile, const _TCHAR* p_pcPath, const _TCHAR* p_pcMode)
{
	uint32_t nError = 0;
	errno_t nCStdErr = 0;

	if(!nError) nError = ASSERTION_POINTER(p_ppFile);
	if(!nError) nError = ASSERTION_POINTER(p_pcPath);
	if(!nError) nError = ASSERTION_POINTER(p_pcMode);

	if(!nError) nCStdErr = _tfopen_s(p_ppFile, p_pcPath,p_pcMode);
	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);	

	if(!nError) nError = ASSERTION_POINTER(*p_ppFile);

	return nError;
}

uint32_t FileClose(std::FILE** p_ppFile)
{
	uint32_t nError = 0;
	int nRet = 0;
	errno_t nCStdErr = 0;

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError && (*p_ppFile)) nRet = fclose(*p_ppFile);
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);
	if(!nError && nRet) nError = TRACE_CSTD(ENOMSG);

	(*p_ppFile) = NULL;

	return nError;
}

uint32_t FileEof(std::FILE* p_pFile, int& p_nRet)
{
	uint32_t nError = 0;
	errno_t nCStdErr = 0;

	if(!nError) nError = ASSERTION_POINTER(p_pFile);

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError) p_nRet = feof(p_pFile);
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);

	return nError;
}

uint32_t FileSeek(std::FILE* p_pFile, long int p_nOffset, int p_nOrigin)
{
	uint32_t nError = 0;
	int nRet = 0;
	errno_t nCStdErr = 0;

	if(!nError) nError = ASSERTION_POINTER(p_pFile);

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError) nRet = fseek(p_pFile,p_nOffset,p_nOrigin);
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);
	if(!nError && nRet) nError = TRACE_CSTD(ENOMSG);

	return nError;
}

uint32_t FileTell(std::FILE* p_pFile, long int& p_nPosition)
{
	uint32_t nError = 0;
	errno_t nCStdErr = 0;

	if(!nError) nError = ASSERTION_POINTER(p_pFile);

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError) p_nPosition = ftell(p_pFile);
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);

	return nError;
}

uint32_t FileGetString(_TCHAR* p_pBuffer, int p_nMaxChar, FILE* p_pFile)
{
	uint32_t nError = 0;
	int nEof = 0;
	errno_t nCStdErr = 0;
	_TCHAR* pRet = NULL;

	if(!nError) nError = ASSERTION_POINTER(p_pBuffer);
	if(!nError) nError = ASSERTION_POINTER(p_pFile);
	if(!nError) nError = FileEof(p_pFile,nEof);
	if(!nError && p_nMaxChar <= 0) nError = TRACE_SYS(STE_ASSERT,_T("Maximum number of characters to read."));
	if(!nError) memset(p_pBuffer,0,p_nMaxChar*sizeof(_TCHAR));

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError && !nEof) pRet = _fgetts(p_pBuffer,p_nMaxChar,p_pFile);
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);
	if(!nError && !nEof && !pRet) nError = FileEof(p_pFile,nEof);
	if(!nError && !nEof && !pRet) nError = TRACE_CSTD(ENOMSG);

	pRet = NULL;
	return nError;
}

uint32_t FileRead(void* p_pBuffer, size_t p_nBufferSize, size_t p_nElementSize, size_t p_nCount, FILE* p_pFile, size_t* p_nReadCount)
{
	uint32_t nError = 0;
	int nEof = 0;
	errno_t nCStdErr = 0;
	size_t nReadCount = 0;

	if(!nError && p_nReadCount) (*p_nReadCount) = 0;

	if(!nError) nError = ASSERTION_POINTER(p_pBuffer);
	if(!nError) nError = ASSERTION_POINTER(p_pFile);
	if(!nError) nError = FileEof(p_pFile,nEof);

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError && !nEof) nReadCount = fread_s(p_pBuffer,p_nBufferSize,p_nElementSize,p_nCount,p_pFile);
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);

	if(!nError && p_nReadCount) (*p_nReadCount) = nReadCount;

	return nError;
}

uint32_t FilePrint(FILE* p_pFile,const _TCHAR* p_pcFormat,...)
{
	uint32_t nError = 0;
	int nRes = 0;
	va_list pArg = NULL;
	errno_t nCStdErr = 0;

	if(!nError) nError = ASSERTION_POINTER(p_pFile);
	if(!nError) nError = ASSERTION_POINTER(p_pcFormat);

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError)
	{
		va_start(pArg, p_pcFormat);
		nRes = _vftprintf_s(p_pFile, p_pcFormat, pArg);
		va_end(pArg);
	}
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);
	if(!nError && nRes < 0) nError = TRACE_CSTD(ENOMSG);

	return nError;
}

uint32_t FileWrite(const void* p_pData, size_t p_nSize, size_t p_nCount, FILE* p_pFile, size_t* p_nWriteCount)
{
	uint32_t nError = 0;
	int nRes = 0;
	va_list pArg = NULL;
	errno_t nCStdErr = 0;

	if(!nError && p_nWriteCount) (*p_nWriteCount) = 0;

	if(!nError) nError = ASSERTION_POINTER(p_pFile);
	if(!nError) nError = ASSERTION_POINTER(p_pData);

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError)
	{
		nRes = fwrite(p_pData,p_nSize,p_nCount,p_pFile);
	}
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);
	if(!nError && nRes != p_nCount) nError = TRACE_CSTD(ENOMSG);

	if(!nError && p_nWriteCount) (*p_nWriteCount) = nRes;

	return nError;
}

uint32_t StrNICmp(const _TCHAR* p_pcS1,const _TCHAR* p_pcS2, size_t p_nCount, int& p_nRet)
{
	uint32_t nError = 0;
	int nRes = 0;
	errno_t nCStdErr = 0;

	p_nRet = 0;

	if(!nError) nError = ASSERTION_POINTER(p_pcS1);
	if(!nError) nError = ASSERTION_POINTER(p_pcS2);
	if(!nError && p_nCount < 0) nError = TRACE_SYS(STE_ASSERT,_T("Number of characters to compare."));

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError)
	{
		nRes = _tcsncicmp(p_pcS1,p_pcS2,p_nCount);
	}
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);
	if(!nError && nRes == _NLSCMPERROR) nError = TRACE_CSTD(ENOMSG);

	if(!nError) p_nRet = nRes;

	return nError;
}

uint32_t StrICmp(const _TCHAR* p_pcS1,const _TCHAR* p_pcS2, int& p_nRet)
{
	uint32_t nError = 0;
	int nRes = 0;
	errno_t nCStdErr = 0;

	p_nRet = 0;

	if(!nError) nError = ASSERTION_POINTER(p_pcS1);
	if(!nError) nError = ASSERTION_POINTER(p_pcS2);

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError)
	{
		nRes = _tcsicmp(p_pcS1,p_pcS2);
	}
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);
	if(!nError && nRes == _NLSCMPERROR) nError = TRACE_CSTD(ENOMSG);

	if(!nError) p_nRet = nRes;

	return nError;
}


uint32_t StrToInt32(const _TCHAR* p_sz, int32_t& p_nValue, int p_nBase)
{
	uint32_t nError = 0;
	TCHAR* pEnd = NULL;
	TCHAR szSpan[] = {_T(" +-1234567890")};
	errno_t nCStdErr = 0;

	if(!nError) nError = ASSERTION_POINTER(p_sz);
	if(!nError && !_tcslen(p_sz)) nError = TRACE_SYS(STE_ASSERT,_T("length"));
	if(!nError && std::_tcsspn(p_sz,szSpan) != _tcslen(p_sz)) nError = TRACE_SYS(STE_ASSERT,_T("span"));

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError)
	{
#ifdef _UNICODE
		p_nValue = std::wcstol(p_sz,&pEnd,p_nBase);
#else
		p_nValue = std::strtol(p_sz,&pEnd,p_nBase);
#endif
	}
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);

	return nError;
}

uint32_t StrToUInt32(const _TCHAR* p_sz, uint32_t& p_nValue, int p_nBase)
{
	uint32_t nError = 0;
	TCHAR* pEnd = NULL;
	errno_t nCStdErr = 0;
	TCHAR szSpan[] = {_T(" +1234567890")};

	if(!nError) nError = ASSERTION_POINTER(p_sz);
	if(!nError && !_tcslen(p_sz)) nError = TRACE_SYS(STE_ASSERT,_T("length"));
	if(!nError && std::_tcsspn(p_sz,szSpan) != _tcslen(p_sz)) nError = TRACE_SYS(STE_ASSERT,_T("span"));

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError)
	{
#ifdef _UNICODE
		p_nValue = std::wcstoul(p_sz,&pEnd,p_nBase);
#else
		p_nValue = std::strtoul(p_sz,&pEnd,p_nBase);
#endif
	}
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);

	return nError;
}

uint32_t StrToFloat(const _TCHAR* p_sz, float& p_fValue)
{
	uint32_t nError = 0;
	TCHAR* pEnd = NULL;
	errno_t nCStdErr = 0;
	TCHAR szSpan[] = {_T(" +-1234567890eE.")};

	if(!nError) nError = ASSERTION_POINTER(p_sz);
	if(!nError && !_tcslen(p_sz)) nError = TRACE_SYS(STE_ASSERT,_T("length"));
	if(!nError && std::_tcsspn(p_sz,szSpan) != _tcslen(p_sz)) nError = TRACE_SYS(STE_ASSERT,_T("span"));

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError)
	{
#ifdef _UNICODE
		p_fValue = std::wcstof(p_sz,&pEnd);
#else
		p_fValue = std::strtof(p_sz,&pEnd);
#endif
	}
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);

	return nError;
}

uint32_t StrToDouble(const _TCHAR* p_sz, double& p_dValue)
{
	uint32_t nError = 0;
	TCHAR* pEnd = NULL;
	errno_t nCStdErr = 0;
	TCHAR szSpan[] = {_T(" +-1234567890eE.")};

	if(!nError) nError = ASSERTION_POINTER(p_sz);
	if(!nError && !_tcslen(p_sz)) nError = TRACE_SYS(STE_ASSERT,_T("length"));
	if(!nError && std::_tcsspn(p_sz,szSpan) != _tcslen(p_sz)) nError = TRACE_SYS(STE_ASSERT,_T("span"));

	if(!nError) nError = SetErrno(nCStdErr);
	if(!nError)
	{
#ifdef _UNICODE
		p_dValue = std::wcstod(p_sz,&pEnd);
#else
		p_dValue = std::strtod(p_sz,&pEnd);
#endif
	}
	if(!nError) nError = GetErrno(nCStdErr);

	if(!nError && nCStdErr) nError = TRACE_CSTD(nCStdErr);

	return nError;
}


} // namespace