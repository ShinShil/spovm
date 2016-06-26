

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <cstring>
#include <stdlib.h>
#include <cassert>
#include <algorithm>

using namespace std;
#ifdef WIN32
	#include <Windows.h>
#endif
#ifdef __linux__
	#include <unistd.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <sys/shm.h> 
	#include <signal.h>
	#include <fcntl.h>	
	#define UINT unsigned int
#endif

#define TRUE 1
#define FALSE 0
#define TRACE_MSG fprintf(stderr, "%s() [%s:%d] here I am\n",__FUNCTION__, __FILE__, __LINE__);