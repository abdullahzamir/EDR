#include "../EDR/ntapi.h"
#include <iostream>


int main(int argc, char **argv) {

	
	HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");

	pNtQueryInformationProcess NtQueryInformationProcess = (pNtQueryInformationProcess)GetProcAddress(hNtdll, "NtQueryInformationProcess");

	//HANDLE hProcess;
	//hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, 8444);

	// Query the process for commandline
	PROCESS_BASIC_INFORMATION pbi;
	ULONG ReturnLength;
	NTSTATUS status;
	status = NtQueryInformationProcess(GetCurrentProcess(), ProcessBasicInformation, &pbi, sizeof(pbi), &ReturnLength);
	PEB peb;
	if (!ReadProcessMemory(GetCurrentProcess(), pbi.PebBaseAddress, &peb, sizeof(peb), NULL)) {
		std::cout << GetLastError() << std::endl;
	}
	else {
		RTL_USER_PROCESS_PARAMETERS upp;
		if (!ReadProcessMemory(GetCurrentProcess(), peb.ProcessParameters, &upp, sizeof(upp), NULL)) {
			std::cout << GetLastError() << std::endl;
		}
		else {

			DWORD dwLength = upp.CommandLine.Length;
			auto cmdline = new WCHAR[dwLength/2 + 1];
			if (!ReadProcessMemory(GetCurrentProcess(), upp.CommandLine.Buffer, cmdline, dwLength, NULL)) {
				std::cout << GetLastError() << std::endl;
			}
			else {
				cmdline[dwLength / 2] = 0;
				std::wcout << cmdline << std::endl;
			}

		}

	}

}