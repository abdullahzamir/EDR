#include <windows.h>
#include <stdio.h>


int main(int argc, char **argv) {
	while (true) {
		Sleep(1000);

		printf("Hello World %s\n", argv[1]);
	}

}