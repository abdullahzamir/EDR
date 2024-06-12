#include "util.h"
#include "Process.h"

int main() {
	Process process;
	std::vector<PROCESSENTRY32> Snap;
	Snap = Process::GetProcessList();

	DEBUG("Number of processes: %d", Snap.size());

	while (true) {
		Sleep(1000);
		bool processChanged = false;
		std::vector<PROCESSENTRY32> NewSnap;
		NewSnap = process.GetProcessList();
		
		for (int i = 0; i < NewSnap.size() - 1; i++) {
			if (Snap[i].th32ProcessID != NewSnap[i].th32ProcessID) {
				DEBUG("Process ID: %d, Process Name %ls", NewSnap[i].th32ProcessID, NewSnap[i].szExeFile);
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