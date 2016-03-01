#pragma once

#define TRUE 1
#define FALSE 0
#ifdef WIN32
	#include <iostream>
	#include <Windows.h>
	#include <math.h>
	#include <tchar.h>
	#include <stdio.h>
	#include <string>
	#ifdef _UNICODE
	typedef std::wstring TSTRING;
	#else
	typedef std::string TSTRING;
	#endif
#endif

#ifdef __linux__
	#include <math.h>
	#include <string.h>
	#include <fstream>
	#include <iostream>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#define TCHAR char
	#define TSTRING string
	#define TEXT
#endif

using namespace std;
