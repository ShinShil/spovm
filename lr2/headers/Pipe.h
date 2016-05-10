#ifndef PIPEH
#define PIPEH

	#define MAX_PIPE_READ_BUFFER 512
	#if defined(WIN32)
		class Pipe {
		private:
			HANDLE hPipe;
		public:
			Pipe() {
				hPipe = NULL;
			}
			~Pipe() {
				CloseHandle(hPipe);
			}
			int create(string pipeName, int openFlag = false) {
				hPipe = CreateFile(
					pipeName.c_str(),
					GENERIC_READ|GENERIC_WRITE,
					1,
					NULL,
					openFlag,
					0,
					NULL
					);
				if(hPipe != INVALID_HANDLE_VALUE)
					return 1;
				if(GetLastError() != ERROR_PIPE_BUSY) {
					cout<<"Impossible to open the pipe"<<endl;
					return -1;
				}
				if(!WaitNamedPipe(pipeName.c_str(), 5000)) {
					cout<<"Waiting for 5 seconds"<<endl;
					return 0;
				}
				return 0;
			}
			int write(string message) {
				if(hPipe == NULL)
					return 0;
				int length = message.length();
				DWORD cbToWrite = (message.length() + 1) * sizeof(char);
				long unsigned int cbWrite = 0; //amount of written bytes
				int fSuccess = WriteFile(
						hPipe,
						message.c_str(),
						length,
						&cbWrite,
						NULL
						);
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
				buffer = chBuff;
				return cbRead;
			}
		};
	#elif defined(_linux_)
	#else
		#error unsupported system
	#endif
#endif