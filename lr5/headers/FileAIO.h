#pragma once

		#define MAX_READ 512

#if defined(WIN32) 
	class FileAIO {
	public:
		static void writeFileA();
		static void readFileA();
	};
#elif defined(__linux__)

#else
	#error Unsupporting system
#endif

