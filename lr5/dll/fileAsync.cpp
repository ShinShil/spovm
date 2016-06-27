#include "FileAsync.h"

	#if defined(WIN32)
		#define MAX_READ 256
		string _read(string path) {
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
		void _write(string path, string src) {
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
		void _test() {
			cout<<"Hello from dll"<<endl;
		}
	#elif defined(__linux__)
		#include <aio.h>
		#define BUF_SIZE 111
		string _read(string path) {
			char buff[BUF_SIZE];
			int fd = open(path.c_str(), O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
			if(fd == -1) {
				string error = "fail to open: ";
				error += path;
				error += ", fileAsync.cpp::read(string) - ";
				error += strerror(errno);
				Debug::error(error);
				return "";
			}
			struct aiocb aio;
			aio.aio_fildes = fd;
			aio.aio_buf = buff;
			aio.aio_nbytes = BUF_SIZE;
			aio.aio_lio_opcode = LIO_WRITE;

			if(aio_read(&aio) == -1) {
				string error = "fileAsync.cpp::aio_read()::==-1 - ";
				error += strerror(errno); 
				Debug::error(error);
				return "";
			}
			int err;
			int ret;
			while(aio_read(&aio) == EINPROGRESS);

			err = aio_error(&aio);
			if(!err) {
				string error = "fileAsync.cpp::aio_read()::!err - ";
				error += strerror(errno); 
				Debug::error(error);
				return "";
			}
			if(ret!=BUF_SIZE) {
				string error = "fileAsync.cpp::aio_read()::ret!=BUF_SIZE - ";
				error += strerror(errno); 
				Debug::error(error);
				return "";
			}
			string res = buff;
			return res;
		}
		void _write(string path, string src) {
			char buff[BUF_SIZE];
			strcpy(buff, src.c_str());
			int fd = open(path.c_str(), O_RDWR | O_EXCL | O_APPEND, S_IRUSR | S_IWUSR);
			if(fd == -1) {
				string error = "fail to open: ";
				error += path;
				error += ", fileAsync.cpp::write(string) - ";
				error += strerror(errno);
				Debug::error(error);
				return;
			}
			struct aiocb aio;
			aio.aio_fildes = fd;
			aio.aio_buf = buff;
			aio.aio_nbytes = BUF_SIZE;

			if(aio_write(&aio) == -1) {
				string error = "fileAsync.cpp::aio_write()::==-1 - ";
				error += strerror(errno); 
				Debug::error(error);
				return;
			}
			int err;
			int ret;
			while(aio_write(&aio) == EINPROGRESS);
			err = aio_error(&aio);
			if(!err) {
				string error = "fileAsync.cpp::aio_write()::!err - ";
				error += strerror(errno); 
				Debug::error(error);
				return;
			}
			if(ret!=BUF_SIZE) {
				string error = "fileAsync.cpp::aio_write()::ret!=BUF_SIZE - ";
				error += strerror(errno); 
				Debug::error(error);
				return;
			}
		}
		void _test() {
			cout<<"test"<<endl;
		}
	#else
		#error Unsupporting system
	#endif