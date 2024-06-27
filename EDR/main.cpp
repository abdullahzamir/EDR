#include "util.h"
#include "Process.h"
#include "Injector.h"

int main() {
	Process process;
	Injector injector;
	std::vector<PROCESSENTRY32> Snap;
	Snap = Process::GetProcessList();

	DEBUG("Number of processes: %d", Snap.size());

	while (true) {
		Sleep(500);
		bool processChanged = false;
		std::vector<PROCESSENTRY32> NewSnap;
		NewSnap = process.GetProcessList();
		
		for (int i = 0; i < NewSnap.size() - 1; i++) {
			if (Snap[i].th32ProcessID != NewSnap[i].th32ProcessID) {
				SUCCESS("PPID: %d PID: %d, Process Name %ls",NewSnap[i].th32ParentProcessID, NewSnap[i].th32ProcessID, NewSnap[i].szExeFile);
				injector.InjectDLL(NewSnap[i].th32ProcessID,L"Hooking.dll");
				process.ProcessTimes(NewSnap[i]);
				process.QueryProcess(NewSnap[i]);
				process.GetProcessCommandline(NewSnap[i]);	
				processChanged = true;
				break;
			}
		}
		if (processChanged) {
			Snap = NewSnap;
		}
	}
	return 0;
}