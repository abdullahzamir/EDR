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
		DEBUG("Process Name: %ls", ProcessName);
	}

	// Query the process for memory permissions
	MEMORY_BASIC_INFORMATION memInfo;
	SIZE_T bytesRead;
	SIZE_T bytesWritten;

	


	if (VirtualQueryEx(hProcess, NULL, &memInfo, sizeof(memInfo)) == 0) {
		ERROR("VirtualQueryEx");
	}

	else {
		DEBUG("meminfo: %lx", memInfo.AllocationProtect);
		DEBUG("%d" , HAS_READ_PERMISSION(memInfo.Protect));
		if (HAS_EXEC_PERMISSION(memInfo.AllocationProtect)) {
			DEBUG("Process has execute permission");
		}
		if (HAS_READ_PERMISSION(memInfo.AllocationProtect)) {
			DEBUG("Process has read permission");
		}
		if (HAS_WRITE_PERMISSION(memInfo.AllocationProtect)) {
			DEBUG("Process has write permission");
		}

	}


}


void Process::GetProcessCommandline(PROCESSENTRY32 pe) {
	
	HANDLE hProcess;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, pe.th32ProcessID);
	if (hProcess == NULL) {
		ERROR("OpenProcess");
	}
	
	else {

		HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");

		pNtQueryInformationProcess NtQueryInformationProcess = (pNtQueryInformationProcess)GetProcAddress(hNtdll, "NtQueryInformationProcess");

		// Query the process for commandline
		PROCESS_BASIC_INFORMATION pbi;
		PEB peb;
		ULONG ReturnLength;
		NTSTATUS status;
		status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &ReturnLength);
		if (status != 0) {
			ERROR("NtQueryInformationProcess");
		}
		else {
			UNICODE_STRING commandLine;
			PVOID rtlUserProcParamsAddress;
			RTL_USER_PROCESS_PARAMETERS upp;
			ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(PVOID), NULL);
			ReadProcessMemory(hProcess, peb.ProcessParameters, &upp, sizeof(upp), NULL);	
			
			ReadProcessMemory(hProcess, upp.CommandLine.Buffer, &commandLine, sizeof(commandLine), NULL);
			WCHAR* commandLineContents = (WCHAR*)malloc(commandLine.Length);
			ReadProcessMemory(hProcess, commandLine.Buffer, commandLineContents, commandLine.Length, NULL);





			CloseHandle(hProcess);

			auto size = commandLine.Length;
			char* chTemp = (char*)malloc(size);
			//wcstombs(chTemp, commandLineContents, size);
			WideCharToMultiByte(CP_ACP, 0, commandLineContents, -1, chTemp, size, 0, 0);
			DEBUG("Commandline: %s", chTemp);
		
		}
	}


}


