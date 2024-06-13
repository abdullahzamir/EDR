#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <winternl.h>
#include "ntapi.h"


#define ANSI_RED "\x1b[31m" 
#define ANSI_GREEN "\x1b[32m" 
#define ANSI_YELLOW "\x1b[33m" 
#define ANSI_BLUE "\x1b[34m" 
#define ANSI_MAGENTA "\x1b[35m" 
#define ANSI_CYAN "\x1b[36m" 
#define ANSI_RESET "\x1b[0m" 


#define DEBUG(...) printf(ANSI_BLUE __VA_ARGS__); printf(ANSI_RESET "\n");
#define ERROR(msg, ...){ \
	printf(ANSI_RED); \
	printf(msg, ##__VA_ARGS__); \
	printf(ANSI_RESET); \
	printf("\n"); \
}

