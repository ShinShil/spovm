#pragma once
#include "main.h"

#if defined( _WIN32)
	#define HSEMAPHORE HANDLE
	class Semaphore {
	private:
		HSEMAPHORE hSem;
		string name;		
	public:
		Semaphore(string name) {
			this->name = name;
		}
		~Semaphore() {
			CloseHandle(hSem);
		}
		void create(int initialCount = 0) {
			hSem = CreateSemaphore(NULL, initialCount, MAXLONG, name.c_str());
			if(hSem == INVALID_HANDLE_VALUE) {
				Debug::debug(GetLastError(), "Semaphore::create(), error code - ");
			}
		}
		void open() {
			hSem = OpenSemaphore(SEMAPHORE_MODIFY_STATE, 0, name.c_str());
			if(hSem == INVALID_HANDLE_VALUE) {
				Debug::debug(GetLastError(), "Semaphore::open(), error code - ");
			}
		}
		void wait(int ntime = INFINITE) {
			int state = WaitForSingleObject(hSem, ntime);
			switch(state) {
				case WAIT_OBJECT_0:
					Debug::debug("sema signaled");
			}
			Debug::debug(state,"state: ");
		}
		void signal(int count = 1) {
			long int prev;
			ReleaseSemaphore(hSem, count, &prev);
			Debug::debug(prev, "prev: ");
		}
	};

#elif defined(__linux__)
	
	#include <semaphore.h>
	#include <fcntl.h>
	#include <sys/stat.h>

	#define HSEMAPHORE sem_t *
	#define MAX_NAME_SEMA 251
	class Semaphore {
	private:
		HSEMAPHORE hSem;
		string name;		
		
	public:
		Semaphore(string name) {
			this->name = "/" + name;
		}
		~Semaphore() {
			sem_destroy(hSem);
		}
		void create(int initialCount = 0) {
			hSem = sem_open(name.c_str(), O_CREAT, S_IRWXU, initialCount);
		}
		void open(int initialCount = 0) {
			hSem = sem_open(name.c_str(), O_CREAT, S_IRWXU, initialCount);
		}
		void wait() {
			sem_wait(hSem);
		}
		void signal(int count = 1) {
			sem_post(hSem);
		}
	};
#else
	#error unsupported system
#endif
 