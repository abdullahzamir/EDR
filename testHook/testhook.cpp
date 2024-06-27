
#include <Windows.h>
#include <iostream>


unsigned char code[] = { 0x53 };



int main() {
	void* exec = VirtualAlloc(0, sizeof code, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	memcpy(exec, code, sizeof code);
	getchar();

	MessageBoxA(NULL, "test", "test", 1);
	CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
	CreateFileA("C:\\Users\\PMLS\\Desktop\\test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


}