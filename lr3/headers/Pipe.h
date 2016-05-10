#ifndef PIPEH
#define PIPEH
#include "Service.h"
#include "MainHeader.h"
	#define MAX_PIPE_READ_BUFFER 512
	#if defined(WIN32)
		#define HPIPE HANDLE
		class Pipe {
		private:
			HPIPE hPipe;
		public:
			Pipe() {
				hPipe = NULL;
			}
			~Pipe() {
				CloseHandle(hPipe);
			}
			int open(string pipeName, int openFlag = CREATE_NEW) {
				pipeName = "\\\\.\\pipe\\"+pipeName;
				hPipe = CreateFile(
					pipeName.c_str(),
					GENERIC_READ|GENERIC_WRITE,
					1,
					NULL,
					OPEN_EXISTING,
					0,
					NULL
					);
//				cout<<"Open the pipe: " << hPipe<<endl;
				if(hPipe != INVALID_HANDLE_VALUE)
					return 1;
//				cout<<"Open the pipe error: " << GetLastError();
				if(GetLastError() != ERROR_PIPE_BUSY) {
					cout<<"Impossible to open the pipe: "<<GetLastError()<<endl;
					return -1;
				}
				if(!WaitNamedPipe(pipeName.c_str(), 5000)) {
//					cout<<"Waiting for 5 seconds"<<endl;
					return 0;
				}
				return 0;
			}
			int create(string pipeName) {
				pipeName = "\\\\.\\pipe\\"+pipeName;
				
				hPipe = CreateNamedPipe(
					pipeName.c_str(),
					PIPE_ACCESS_DUPLEX,
					PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
					10,
					MAX_PIPE_READ_BUFFER + 4,
					MAX_PIPE_READ_BUFFER + 4,
					20000,
					NULL
					);
//				cout<<"Create the pipe: "<<hPipe<<endl;
			}
			int write(string message) {
				int fSuccess;
				DWORD dwMode;
				if(hPipe == NULL)
					return 0;
				
				int length = message.length();
//					cout<<"Write before error: "<<GetLastError()<<endl;

				DWORD cbToWrite = (message.length() + 1) * sizeof(char);
				long unsigned int cbWrite = 0; //amount of written bytes
				fSuccess = WriteFile(
						hPipe,
						message.c_str(),
						length + 1,
						&cbWrite,
						NULL
						);
				if(!fSuccess)
//					cout<<"Write error: "<<GetLastError()<<endl;
				return fSuccess;
			}
			int read(string& buffer) {
				if(hPipe == NULL)
					return 0;
				char chBuff[MAX_PIPE_READ_BUFFER];
				DWORD cbRead; //amount of read bytes

				int fSuccess = ReadFile(
						hPipe,
						chBuff,
						MAX_PIPE_READ_BUFFER * sizeof(char),
						&cbRead,
						NULL
					);
				if(fSuccess)
					buffer = chBuff;
				buffer[cbRead] = '\0';
				/*buffer[cbRead+1] = '\0';
				buffer[cbRead+2] = '\0';*/

//				cout<<"Open error: "<<GetLastError()<<endl;
				return cbRead;
			}
		};
	#elif defined(__linux__)
		#include <unistd.h>
		#define HPIPE int
		#define READ 0
		#define WRITE 1
		class Pipe {
		private:
			int pipefd[2];
			HPIPE hPipe;
		public:
			Pipe() {

			}
			~Pipe() {

			}
			int open(string pipeName, int openFlag = CREATE_NEW) {
				return pipe(pipefd);
			}
			int write(string message) {
				//fdopen(pipefd[WRITE], "w");
				close(pipefd[READ]);
				unistd::write(pipefd[0], message.c_str(), message.length() + 1);
				close(pipefd[1]);
				return 0;
			}
			int read(string& buffer) {
				char buf;
				int i = 0;
				//fdopen(pipefd[READ], "r");
				close(pipefd[WRITE]);
				while(unistd::read(pipefd[0], &buf, 1) > 0) {
					buffer[i] = buf;
					++i;
				}
				buffer[i] = '\0';
				return 0;
			}
		};
	#else
		#error unsupported system
	#endif
#endif