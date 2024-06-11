#include "Process.h"
#include "util.h"


#define HAS_EXEC_PERMISSION(prot) (prot & 0xF0)
#define HAS_READ_PERMISSION(prot) (prot  == 0x08 || prot == 0x80)
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


void Process::QueryProcess(DWORD pid) {
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);


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

	DWORD prot = 0;


	if (VirtualQueryEx(hProcess, NULL, &memInfo, sizeof(memInfo)) == 0) {
		ERROR("VirtualQueryEx");
	}
	else {
		DEBUG("%d" , HAS_READ_PERMISSION(memInfo.Protect));
		if (HAS_EXEC_PERMISSION(memInfo.Protect)) {
			DEBUG("Process has execute permission");
		}
		if (HAS_READ_PERMISSION(memInfo.Protect)) {
			DEBUG("Process has read permission");
		}
		if (HAS_WRITE_PERMISSION(memInfo.Protect)) {
			DEBUG("Process has write permission");
		}

	}


}