#include "Process.h"
#include "util.h"


#define HAS_EXEC_PERMISSION(prot) (prot & PAGE_EXECUTE)
#define HAS_READ_PERMISSION(prot) (prot  == PAGE_READWRITE || prot == 0x80)
#define HAS_WRITE_PERMISSION(prot) (prot & 0xCC)


Process::Process() {
	// Constructor
}

Process::~Process() {
	// Destructor
}

std::vector<PROCESSENTRY32> Process::GetProcessList() {
	// Get the list of processes
	std::vector<PROCESSENTRY32> ProcessList;
	HANDLE hProcessSnap;
	UINT32 NumberOfProcesses = 0;
	
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);


		LPPROCESSENTRY32 pe32;
		pe32 = new PROCESSENTRY32;
		pe32->dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(hProcessSnap, pe32)) {
			ERROR("Process32First");
		}

		while (Process32Next(hProcessSnap, pe32)) {
			ProcessList.push_back(*pe32);		
		}
		return ProcessList;
	}


void Process::QueryProcess(PROCESSENTRY32 pe) {
	HANDLE hProcess;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, pe.th32ProcessID);


	// Query the process for name 

	if (hProcess == NULL) {
		ERROR("OpenProcess");
	}
	else {
		TCHAR ProcessName[MAX_PATH];
		DWORD ProcessNameLength = MAX_PATH;
		QueryFullProcessImageName(hProcess, 0, ProcessName, &ProcessNameLength);
		DEBUG("Executable Path: %ls", ProcessName);
	}

	MEMORY_BASIC_INFORMATION mbi = {};
	LPVOID offset = 0;
	HANDLE process = NULL;
	uint8_t buffer[16] = { 0 };
		process = OpenProcess(MAXIMUM_ALLOWED, false, pe.th32ProcessID);
		if (process)
		{
			std::wcout << pe.szExeFile << "\n";
			while (VirtualQueryEx(process, offset, &mbi, sizeof(mbi)))
			{
				offset = (LPVOID)((DWORD_PTR)mbi.BaseAddress + mbi.RegionSize);
				if (mbi.AllocationProtect == PAGE_EXECUTE_READWRITE && mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE)
				{
					std::cout << "\tRWX: 0x" << std::hex << mbi.BaseAddress << "\n";
					ReadProcessMemory(process, mbi.BaseAddress, buffer, 16, NULL);
					std::cout << "\t\tData: ";
					for (int i = 0; i < 16; i++)
					{
						std::cout << std::hex << (int)buffer[i] << " ";
					}
					std::cout << std::endl;
				}
			}
			offset = 0;
		}
		CloseHandle(process);
	
}


void Process::GetProcessCommandline(PROCESSENTRY32 pe) {
	

	HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");

	pNtQueryInformationProcess NtQueryInformationProcess = (pNtQueryInformationProcess)GetProcAddress(hNtdll, "NtQueryInformationProcess");

	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
	if (hProcess == NULL) {
		ERROR("OpenProcess");
	}
	
	else {
		PROCESS_BASIC_INFORMATION pbi;
		ULONG ReturnLength;
		NTSTATUS status;
		status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &ReturnLength);
		PEB peb;
		if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), NULL)) {
			ERROR("%s", GetLastError());
		}
		else {
			RTL_USER_PROCESS_PARAMETERS upp;
			if (!ReadProcessMemory(hProcess, peb.ProcessParameters, &upp, sizeof(upp), NULL)) {
				ERROR("%s", GetLastError());
			}
			else {

				DWORD dwLength = upp.CommandLine.Length;
				auto cmdline = new WCHAR[dwLength / 2 + 1];
				if (!ReadProcessMemory(hProcess, upp.CommandLine.Buffer, cmdline, dwLength, NULL)) {
					ERROR("%s", GetLastError());
				}
				else {
					cmdline[dwLength / 2] = 0;
					SUCCESS("Commandline: %ls", cmdline);
				}

			}

		}

		
	}


}

void Process::ProcessTimes(PROCESSENTRY32 pe) {
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
	if (hProcess == NULL) {
		ERROR("OpenProcess");
	}
	else {
		FILETIME CreationTime;
		FILETIME ExitTime;
		FILETIME KernelTime;
		FILETIME UserTime;
		if (!GetProcessTimes(hProcess, &CreationTime, &ExitTime, &KernelTime, &UserTime)) {
			ERROR("GetProcessTimes");
		}
		else {
			// Convert FILETIME to time_t
			SYSTEMTIME st;
			FILETIME ft;
			LARGE_INTEGER li;
			li.LowPart = CreationTime.dwLowDateTime;
			li.HighPart = CreationTime.dwHighDateTime;
			FileTimeToSystemTime(&CreationTime, &st);
			
			SUCCESS("Creation Time: %d:%d:%d", st.wHour, st.wMinute, st.wSecond);
		}
	}
}


