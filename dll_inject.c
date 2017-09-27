#include <stdio.h>
#include <windows.h>
//#include "dll.h"

DWORD WINAPI MyThread();
DWORD threadID;
void __stdcall FunctionCall(void);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD reason, LPVOID lpvReserved) {

    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread(NULL, 0, &MyThread, NULL, 0, &threadID);
            break;

        case DLL_PROCESS_DETACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }

    return TRUE;
} 

DWORD WINAPI MyThread()
{
    while(1)
    {
        if(GetAsyncKeyState(VK_F4) & 1)
        {
            FunctionCall();
        }
        else if(GetAsyncKeyState(VK_F5) & 1)
            break;
    Sleep(100);
    }
    return 0;
}

void __stdcall FunctionCall(void)
{
    typedef void (__stdcall *pFunctionAddress)();
    pFunctionAddress pWinFunction = (pFunctionAddress)(0x01001B81);
    pWinFunction(); 
}