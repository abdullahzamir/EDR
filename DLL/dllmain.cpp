// dllmain.cpp : Defines the entry point for the DLL application.
#pragma comment(lib, "detours.lib")

#include "pch.h"
#include <detours.h>



INT(WINAPI* oMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT) = MessageBoxA;


int WINAPI hookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    lpText = "Hooked!";
    return oMessageBoxA(hWnd, lpText, lpCaption, uType);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)oMessageBoxA, hookedMessageBoxA);
        DetourTransactionCommit();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

