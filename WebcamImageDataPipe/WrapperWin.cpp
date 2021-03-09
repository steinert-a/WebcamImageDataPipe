#include "PcHeader.h"

#include "Trace.h"
#include "TraceSys.h"
#include "TraceWin.h"

#include "SystemTrace.h"

#include "WrapperWin.h"


namespace wrap_win
{
uint32_t ComInitialize(DWORD p_dwCoInit)
{
	HRESULT hr = S_OK;
	uint32_t nError = 0;

	if(!nError) hr = CoInitializeEx(NULL, p_dwCoInit);
	if(!nError && FAILED(hr)) nError = TRACE_WIN(hr,_T("COM"));

	return nError;
}

void ComUninitialize()
{
	CoUninitialize();
}

uint32_t ComCreateInstance(REFCLSID p_rclsid, LPUNKNOWN p_pUnkOuter, DWORD p_dwClsContext, REFIID p_riid, LPVOID* p_ppv)
{
	HRESULT hr = S_OK;
	uint32_t nError = 0;

	if(!nError) hr = CoCreateInstance(p_rclsid, p_pUnkOuter, p_dwClsContext, p_riid, p_ppv);
	if(!nError && FAILED(hr)) nError = TRACE_WIN(hr,_T(""));

	return nError;
}

} // namespace