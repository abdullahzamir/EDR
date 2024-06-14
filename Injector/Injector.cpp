#include "../EDR/util.h"


void InjectDLL(DWORD pid, LPCTSTR dllPath) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	LPVOID pRemoteBuffer;
	pRemoteBuffer = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(hProcess, pRemoteBuffer, dllPath, (wcslen(dllPath) + 1) * sizeof(TCHAR), NULL);
	LPTHREAD_START_ROUTINE  pThreadProc;
	
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuffer, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, pRemoteBuffer, 0, MEM_RELEASE);
}

int main(int argc, char **argv) {
	
	int pid = atoi(argv[1]);
	InjectDLL(pid,L"Hooking.dll");
	return 0;
}