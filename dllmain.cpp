// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" {
    __declspec(dllexport) BOOL WINAPI DllRunner(void) {
        const wchar_t* text1 = L"Hello from s4nsec!";
        const wchar_t* caption1 = L"Warm Greetings";

        ::MessageBox(NULL, text1, caption1, MB_OK);
        return TRUE;
    }

}
