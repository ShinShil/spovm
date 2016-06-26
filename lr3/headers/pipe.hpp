#pragma once
#include "main.h"
#include "debug.hpp"

#define PIPE_OUT_SIZE 254
#define PIPE_IN_SIZE 254
#define BUFFER_READ PIPE_IN_SIZE
#define BUFFER_WRITE PIPE_OUT_SIZE
#define PIPE_DFLT_TIMEoUT INFINITE
#if defined(WIN32) 
	#define HPIPE HANDLE
	class Pipe {
	private:
		HPIPE hPipe;
		string name;
		string pipeName;
	public:
		Pipe(string name) {
			this->name = name;
			this->pipeName = "\\\\.\\pipe\\"+name;
			hPipe = NULL;			
		}
		~Pipe() {
			if(hPipe) 
				CloseHandle(hPipe);
		}	
		void create() {
			hPipe = CreateNamedPipe(this->pipeName.c_str(), PIPE_ACCESS_DUPLEX, 0, PIPE_UNLIMITED_INSTANCES, PIPE_OUT_SIZE, PIPE_IN_SIZE, PIPE_DFLT_TIMEoUT, NULL);
			if(hPipe == INVALID_HANDLE_VALUE) {
				Debug::debug(GetLastError(), "Pipe::create(), error code - ");
			}
		}
		void connectClient() {
			hPipe = CreateFile(this->pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if(hPipe == INVALID_HANDLE_VALUE) {
				Debug::debug(GetLastError(), "Pipe::connectClient(), error code - ");
			}
		}
		string read() {
			char buffer[BUFFER_READ] = {0};
			long unsigned int readenBytes;
			ReadFile(hPipe, buffer, BUFFER_READ-1, &readenBytes, NULL);
			string res = buffer;
			return res;

		}
		void write(string data) {
			long unsigned int wroteBytes;
			int error = WriteFile(hPipe, data.c_str(), data.length() + 1, &wroteBytes, NULL);
			if(!error) {
				Debug::debug(GetLastError(), "Pipe::write(), error code - ");
			}
		}
	};
#elif defined(__linux__)
	#define READ_END 0
	#define WRITE_END 1
			//LinuxWrapper
		class LW {
		public:
			static int _create(const char* path, int mode) {
				return creat(path, mode);
			}
			static int _read(int fd, void* buffer, int readBytes) {
				return read(fd, buffer, readBytes);
			}
			static int _write(int fd, const void* data, int writeBytes) {
				return write(fd, data, writeBytes);
			}
		};
	
	class Pipe {
	private:
		string name;
		string fileWrite;
		string fileRead;
		int pipefd[2];
	public:
		Pipe(string name) {
			this->name = name;
			this->fileWrite = "pipeWrite" + name + ".txt";
			this->fileRead = "pipeRead" + name + ".txt";
		}
		~Pipe() {
			close(pipefd[WRITE_END]);
			close(pipefd[READ_END]);
		}
		void create() {
			pipefd[WRITE_END] = LW::_create(fileWrite.c_str(), S_IWUSR);
			if(pipefd[WRITE_END] == -1) {
				Debug::error(errno, "Pipe::create() - ");
			}

		}
		void connectClient() {
			pipefd[READ_END] = LW::_create(fileRead.c_str(), S_IRUSR);
			if(pipefd[READ_END] == -1) {
				Debug::error(errno, "Pipe::connectClient() - ");
			}
		}
		string read() {
			char buff[BUFFER_READ];
			int err = LW::_read(pipefd[READ_END], buff, BUFFER_READ - 1);
			if(err == -1) {
				Debug::error(errno, "Pipe::read() - ");
				return "";
			}
			string res = buff;
			return res;
		}
		void write(string data) {
			int err = LW::_write(pipefd[WRITE_END], data.c_str(), BUFFER_WRITE);
			if(err == -1) {
				Debug::error(errno, "Pipe::write() - ");
			}
		}
	};
#else 
	#error unsupported system
#endif