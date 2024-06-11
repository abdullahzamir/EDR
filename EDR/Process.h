
#include "util.h"

class Process {
public:
	Process();
	~Process();
	static std::vector<PROCESSENTRY32> GetProcessList();
	void QueryProcess(PROCESSENTRY32);
	void GetProcessCommandline(PROCESSENTRY32);
};
