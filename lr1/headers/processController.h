#pragma once
#include "MainHeader.h"

class Process {
private:
#ifdef WIN32
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
#endif
	
	TCHAR* path;
public:
	Process(TCHAR* path);
	bool start(TCHAR* commandLine);
	int waitEnd();
};