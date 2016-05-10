#ifndef SEMAPHOREH
#define SEMAPHOREH
#include "MainHeader.h"
#include "Service.h"
#ifdef __linux__
	
#endif	

	#if defined( _WIN32)
		#define HSEMAPHORE HANDLE
		class Semaphore {
		private:
			HSEMAPHORE hSem;		
		public:
			Semaphore() {
			}
			~Semaphore() {
				CloseHandle(hSem);
			}
			void create(string name,int initialCount = 0) {
				hSem = CreateSemaphore(NULL, initialCount, MAXLONG, name.c_str());
//				cout<<"Create sema:" << hSem<<endl;
			}
			void open(string name) {
				hSem = OpenSemaphore(SEMAPHORE_MODIFY_STATE, 0, name.c_str());
//				cout<<"Open: "<<hSem<<endl;
//				cout<<"Open sema erro: "<<GetLastError()<<endl;
			}
			void wait() {
				WaitForSingleObject(hSem, INFINITE);
			}
			void signal(int count = 1) {
				ReleaseSemaphore(hSem, count, NULL);
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
		public:
			Semaphore() {
			}
			~Semaphore() {
				sem_destroy(hSem);
			}
			void create(string name,int initialCount = 0) {
				name = "/" + name;
				hSem = sem_open(name.c_str(), O_CREAT, S_IRWXU, initialCount);
				cout<<"Create semaphore:" << hSem<<endl;
			}
			void open(string name) {
				name = "/" + name;
				hSem = sem_open(name.c_str(), O_CREAT, S_IRWXU, initialCount);
				cout<<"Open semaphore: "<<hSem<<endl;
				Service::printLastError("Open sema error: ");
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
#else
#endif //SEMAPHOREH