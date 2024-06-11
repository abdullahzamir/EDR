
#include "util.h"

class Process {
public:
	Process();
	~Process();
	std::vector<PROCESSENTRY32> GetProcessList();
	void QueryProcess(DWORD pid);
};
