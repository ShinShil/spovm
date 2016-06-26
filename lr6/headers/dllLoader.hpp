#pragma once
#include "main.h"

#if defined(WIN32)
	#define HDLL HINSTANCE
	#define DLL_FUNC_TYPE FARPROC
	class DllLoader {
	public:
		static HDLL load(string name) {
			name += ".dll";
			return LoadLibrary(name.c_str());
		}
		static DLL_FUNC_TYPE getFunc(HDLL dll, string name) {
			return GetProcAddress(dll, name.c_str());
		}
		static void free(HDLL dll) {
			FreeLibrary(dll);
		}
	};
#elif defined(__linux__)
#include <dlfcn.h>
	#define HDLL void*
	#define DLL_FUNC_TYPE void*
	class DllLoader {
	public:
		static HDLL load(string name) {
			name += ".dll";
			return dlopen(name.c_str(), RTRD_NOW);
		}
		static DLL_FUNC_TYPE getFunc(HDLL dll, string name) {
			return dlsym(dll, name.c_str());
		}
		static void free(HDLL dll) {
			dlclose(dll);
		}
	};
#else 
	#error unsupported system
#endif