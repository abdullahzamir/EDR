// dllmain.cpp : Defines the entry point for the DLL application.
#pragma comment(lib, "detours.lib")

#include "pch.h"
#include <detours.h>
#include "../EDR/util.h"

BOOL(WINAPI* oCreateProcess)(LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION) = CreateProcessA;
INT(WINAPI* oMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT) = MessageBoxA;
HANDLE(WINAPI* oCreateFileA)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE) = CreateFileA;

int WINAPI hookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    lpText = "Hooked!";

    return oMessageBoxA(hWnd, lpText, lpCaption, uType);
}

BOOL WINAPI hookedCreateProcess(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
    DEBUG("CreateProcessA called with lpApplicationName: %s\n", lpApplicationName);
    DEBUG("CreateProcessA called with lpCommandLine: %s\n", lpCommandLine);
	return oCreateProcess(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
}

HANDLE WINAPI hookedCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	DEBUG("CreateFileA called with lpFileName: %s\n", lpFileName);
	return oCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
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
        DetourAttach(&(PVOID&)oCreateProcess, hookedCreateProcess);
        DetourAttach(&(PVOID&)oCreateFileA, hookedCreateFileA);
        DetourTransactionCommit();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

