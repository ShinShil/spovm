#pragma once
#include "main.h"
#include "debug.hpp"

typedef struct ThreadData {
		char* str;
		int length;
} THREAD_DATA, *PTHREAD_DATA;

#if defined(WIN32)
	#define HTREAD HANDLE
	#define THREAD DWORD WINAPI
	#define FUNC_THREAD_TYPE LPTHREAD_START_ROUTINE
	#define PARAMS_THREAD_TYPE LPVOID
	#define NOT_EXIT	-1
	

	class Thread {
	private:
		HTREAD hThread;
		FUNC_THREAD_TYPE func;
		PARAMS_THREAD_TYPE params;
		int exitCode;
		int nextExitCode;
	public:
		Thread(FUNC_THREAD_TYPE func) {
			this->hThread = NULL;
			this->func = func;
			this->params = NULL;
			this->exitCode = NOT_EXIT;	
		}
		Thread(FUNC_THREAD_TYPE func, PARAMS_THREAD_TYPE params) {
			this->hThread = NULL;
			this->func = func;
			this->params = params;
			this->exitCode = NOT_EXIT;	
		}
		~Thread() {
			if(hThread) CloseHandle(hThread);
		}
		//start adn not waiting
		void startAsync() {
			if(hThread) CloseHandle(hThread);
			//default stackSize = 1mb, second arg; last - runn immediatly
			hThread = CreateThread(NULL, 0, this->func, this->params, 0, NULL);
			if(hThread == NULL) {
				Debug::error(GetLastError(), "Thread::run() - ");
			}else {
				this->exitCode = -1;
			}
		}
		//starts and don't let to run antoher thread until it ended
		void startSync() {
			if(hThread) CloseHandle(hThread);
			//default stackSize = 1mb, second arg; last - runn immediatly
			hThread = CreateThread(NULL, 0, this->func, this->params, 0, NULL);
			if(hThread == NULL) {
				Debug::error(GetLastError(), "Thread::run() - ");
			}else {
				this->exitCode = -1;
			}
			WaitForSingleObject(hThread, INFINITE);	
		}	
		void stop(int exitCode = 0) {
			if(!hThread) return;
			this->exitCode = exitCode;
			TerminateThread(hThread, exitCode);
		}
		static PTHREAD_DATA packThreadData(string data) {
			PTHREAD_DATA pData = (PTHREAD_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(THREAD_DATA));
			char* str = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, data.length());
			strcpy(str, data.c_str());
			pData -> str = str;
			pData -> length = data.length();
			return pData;
		}
		static string unpackThreadData(void* data) {
			if(data != NULL) {
				PTHREAD_DATA pData = (PTHREAD_DATA)data;
				string str = pData->str;
				return str;
			}
			return "";
		}
	};
#elif defined(__linux__)
	#include <pthread.h>
	#define HTHREAD pthread_t
	#define THREAD int
	#define PARAMS_THREAD_TYPE void*

	typedef THREAD (*FUNC_THREAD_TYPE)(void*);

	typedef struct ThreadFunc {
		FUNC_THREAD_TYPE func;	
		void* params;
	} THREAD_FUNC_DATA, *PTHREAD_FUNC_DATA;

	class Thread {
	private:
		HTHREAD hThread;
		PTHREAD_FUNC_DATA funcData;
		static void* funcWrapper(void* params) {
			PTHREAD_FUNC_DATA funcData = (PTHREAD_FUNC_DATA)params;
			funcData->func(funcData->params);
		}

	public:
		Thread(FUNC_THREAD_TYPE func) {
			this->funcData = (PTHREAD_FUNC_DATA)malloc(sizeof(THREAD_FUNC_DATA));
			this->funcData->func = func;
			this->funcData->params = NULL;
		}
		Thread(FUNC_THREAD_TYPE func, PARAMS_THREAD_TYPE params) {
			this->funcData = (PTHREAD_FUNC_DATA)malloc(sizeof(THREAD_FUNC_DATA));
			this->funcData->func = func;
			this->funcData->params = params;
		}
		~Thread() {

		}
		void startAsync() {
			int err = pthread_create(&hThread, NULL, funcWrapper, this->funcData);
			if(err) {
				Debug::error(errno, "Thread::startAsync() - ");
				return;
			}
		}
		void startSync() {
			int err = pthread_create(&hThread, NULL, funcWrapper, this->funcData);
			if(err) {
				Debug::error(errno, "Thread::startSync() - ");
				return;
			}
			pthread_join(hThread, NULL);
		}
		void stop() {

		}
		static PTHREAD_DATA packThreadData(string data) {
			PTHREAD_DATA pData = (PTHREAD_DATA)malloc(sizeof(THREAD_DATA));
			char* str = (char*)malloc(data.length());
			strcpy(str, data.c_str());
			pData -> str = str;
			pData -> length = data.length();
			return pData;
		}
		static string unpackThreadData(void* data) {
			if(data != NULL) {
				PTHREAD_DATA pData = (PTHREAD_DATA)data;
				string str = pData->str;
				return str;
			}
			return "";
		}
	};
#else
	#error unsupported system
#endif