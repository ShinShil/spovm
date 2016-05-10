#pragma once
#ifndef NS_THREADH
#define NS_THREADH


	#if defined(_WIN32)
		#include <process.h>

		#include "MainHeader.h"
	
		#define HTREAD HANDLE
		#define THREAD_START_ROUTINE DWORD WINAPI
		#define LPTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE
		#define THREAD_START_ROUTINE_TYPE DWORD WINAPI
		#define THREAD_START_ROUTINE_ARGSTYPE PVOID
		#define ARGS_THREAD PVOID 
		#define IDTRHREAD DWORD
		class Ns_thread {
		private:
			HTREAD hThread;
			IDTRHREAD idThread;
		public:
			Ns_thread() {
				hThread = NULL;
			}
			~Ns_thread() {
				ExitThread(0);
				CloseHandle(hThread);
			}
			void create(ARGS_THREAD args, LPTHREAD_START_ROUTINE start_routine, int mode = 0) {
				if(mode) {
					mode = CREATE_SUSPENDED;
				}
				hThread = CreateThread(NULL, 0, start_routine, args, mode, &idThread);
			}
			void wait() {
				WaitForSingleObject(hThread, INFINITE);
			}
			void destroy() {
				TerminateThread(hThread, 0);
				CloseHandle(hThread);
			}
		};
	#elif defined(__linux__)

		#include <pthread.h>

		#define HTREAD pthread_t 
		#define ARGS_THREAD void* 
		#define THREAD_START_ROUTINE void *
		#define LPTHREAD_START_ROUTINE FUNC_TYPE
		#define THREAD_START_ROUTINE_TYPE void*
		#define THREAD_START_ROUTINE_ARGSTYPE void*
		typedef void* (*FUNC_TYPE)(THREAD_START_ROUTINE_ARGSTYPE);

		class Ns_thread {
		private:
			HTREAD hThread;
		public:
			Ns_thread() {
				hThread = 0;
			}
			~Ns_thread() {

			}
			void create(ARGS_THREAD args, LPTHREAD_START_ROUTINE start_routine, int mode = 0) {
				pthread_create(&hThread, NULL, start_routine, args);
			}
			void wait() {
				pthread_join(hThread, NULL);
			}
			void destroy() {
				pthread_cancel(hThread);
			}
		};
	#else
		#error Unsopporting system
	#endif
#endif