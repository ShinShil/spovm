#pragma once
#include "MainHeader.h"

#ifdef WIN32
#define HPROCESS HANDLE
#define HTHREAD HANDLE
#endif
#ifdef __linux__
#define HPROCESS int
#define HTHREAD int
#endif

#define RESUME 0
#define PAUSE 1

class Process {
private:
	HPROCESS hProcess;
	HTHREAD hMainThread;
	int state;
	static int linStatus;
public:
	string name;

	HPROCESS getHProcess();

	int getState();
	static void waitSignal(HPROCESS hProcess, UINT time);
	static HPROCESS getHandle(string name);
    static HPROCESS getCurrProcessHandle();
	void create(string cmdline);
	void resume();
	void pause();
	void close();
};