#pragma once

namespace wrap_win
{
uint32_t ComInitialize(DWORD);
void ComUninitialize();
uint32_t ComCreateInstance(REFCLSID,LPUNKNOWN,DWORD,REFIID,LPVOID*);

}