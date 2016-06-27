#pragma once
#include "main.h"
#include "debug.hpp"

#if defined(WIN32)

extern "C" __declspec(dllexport) string _read(string path);
extern "C" __declspec(dllexport) void _write(string path, string src);
extern "C" __declspec(dllexport) void _test();

#elif defined(__linux__)
	extern "C" string _read(string path);
	extern "C" void _write(string path, string src);
	extern "C" void _test();

#else 
	#error unsupported system
#endif