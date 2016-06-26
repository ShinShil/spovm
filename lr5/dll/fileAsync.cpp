#include "FileAsync.h"

	#if defined(WIN32)
		#define MAX_READ 256
		string read(string path) {
			DWORD readenByte;
			int t;
			char pBuffer[MAX_READ] = {0};
			HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			OVERLAPPED overlap = {0};
			overlap.hEvent = hEvent;
			HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
			if(!hFile) {
				Debug::error(GetLastError(), "fileAsync.cpp::read - ");
			}
			t = ReadFile(hFile, pBuffer, sizeof(pBuffer),NULL, &overlap);
			int nError = GetLastError();
			if(!t && nError != ERROR_IO_PENDING) {
				Debug::error(GetLastError(), "fileAsync.cpp::read - ");
			}
			t = WaitForSingleObject(hEvent, INFINITE);
			if(t == WAIT_TIMEOUT) {
				Debug::error("fail to read - timeout, fileAsync.cpp::read");
			}
			Debug::debug("read");
			GetOverlappedResult(hFile, &overlap, &readenByte, FALSE);
			pBuffer[readenByte] = '\0';
			string str = pBuffer;
			CloseHandle(hFile);
			return str;
		}
		void write(string path, string src) {
			DWORD readenByte;
			Debug::debug(src);
			char* buffer = new char[src.length() + 1];
			strcpy(buffer, src.c_str());
			HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			OVERLAPPED overlap = {0};
			overlap.hEvent = hEvent;
			HANDLE hFile = CreateFile(path.c_str(), FILE_APPEND_DATA, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
			if(hFile == INVALID_HANDLE_VALUE) {
				Debug::error(GetLastError(), "fileAsync.cpp::write::41 - ");
			}
			int t = WriteFile(hFile, buffer, src.length() + 1, NULL, &overlap);
			int nError = GetLastError();
			if(!t && nError != ERROR_IO_PENDING) {
				Debug::error(GetLastError(), "fileAsync.cpp::write - ");
			}
			t = WaitForSingleObject(hEvent, INFINITE);
			if(t == WAIT_TIMEOUT) {
				Debug::error("fail to write - timeout, fileAsync.cpp:write");
			}
			CloseHandle(hFile);
			delete[] buffer;
		}
		void test() {
			cout<<"Hello from dll"<<endl;
		}
	#elif defined(__linux__)

	#else
		#error Unsupporting system
	#endif