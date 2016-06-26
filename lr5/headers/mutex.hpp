#pragma once
#include "main.h"
#include "debug.hpp"

#if defined(WIN32) 
	#define HMUTEX HANDLE
	class Mutex {
	private:
		HMUTEX hMutex;
		string name;
	public:
		Mutex() {
			hMutex = CreateMutex(NULL, FALSE, NULL);
			if(hMutex == NULL) {
				Debug::error(GetLastError(), "Mutex::Mutex() - ");
			}
		}
		~Mutex() {
			if(hMutex != NULL) {
				CloseHandle(hMutex);
			}
		}
		void wait() {
			if(hMutex != NULL) {
				int res = WaitForSingleObject(hMutex, INFINITE);
			}
		}
		void unlock() {
			int res = ReleaseMutex(hMutex);
			if(!res) {
				Debug::error(GetLastError(), "Mutex::lock() - ");
			}
		}
	};
#elif defined(__linux__)
	#define HMUTEX pthread_mutex_t
	class Mutex {
	private:
		HMUTEX hMutex;
	public:
		Mutex() {
			hMutex = PTHREAD_MUTEX_INITIALIZER;
		}
		~Mutex() {

		}
		void wait() {
			pthread_mutex_lock(&hMutex);
		}
		void unlock() {
			pthread_mutex_unlock(&hMutex);
		}
	};
#else 
	#error unsupported system
#endif