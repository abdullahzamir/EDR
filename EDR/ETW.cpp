#include "ETW.h"
#include "util.h"


void ETW::StartETW() {
	// Start ETW
	EVENT_TRACE_PROPERTIES* properties;
	TRACEHANDLE hTrace = 0;

	memset(&properties, 0, sizeof(EVENT_TRACE_PROPERTIES));
	properties->Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
	properties->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
	properties->Wnode.ClientContext = 1;
	properties->Wnode.Guid = { 0x3d6fa8d0, 0xfe05, 0x11d0, {0x9f, 0x1f, 0x00, 0x00, 0xf8, 0x03, 0xa1, 0x09} };
	properties->BufferSize = 64;
	properties->MinimumBuffers = 2;
	properties->MaximumBuffers = 2;
	properties->NumberOfBuffers = 2;
	properties->LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
	properties->LoggerNameOffset = 0;
	properties->LogFileNameOffset = 0;


	ULONG status = StartTrace(&hTrace, L"ETW", properties);

	EnableTraceEx(&properties->Wnode.Guid, 0, 0, TRUE, TRACE_LEVEL_INFORMATION, 0, 0, 0, 0);


}