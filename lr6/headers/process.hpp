#pragma once 
#include "main.h"

#if defined(WIN32)
	#include <tlhelp32.h>
	 #define HPROCESS HANDLE
	class Process {
	private:
		HANDLE hProcess;
		string nameCmdLine;
		Process(HANDLE hProcess) {
			this->hProcess=hProcess;
			exitCode = -1;
		}
		long unsigned int exitCode;
	public:
		Process(string nameCmdLine, int startFlag = FALSE) {
			this->nameCmdLine = nameCmdLine;
			if(startFlag)
				this->start();
			exitCode = -1;
		}
		~Process() {
			CloseHandle(hProcess);
		}
		void start() {
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			si.cb = sizeof(si);
			ZeroMemory(&si, sizeof(PROCESS_INFORMATION));
			char* tmp = new char[255];
			strcpy(tmp, nameCmdLine.c_str());
			CreateProcess(NULL, tmp, NULL, NULL,FALSE,0,NULL,NULL,&si,&pi);
			hProcess = pi.hProcess;
		}
		void stop() {
			TerminateProcess(hProcess, NO_ERROR);
		}
		
		static Process* takeExisting(string name) {
			HANDLE hProcess = GetProcessByName(name);
			return hProcess == NULL ? NULL : new Process(hProcess);
		}

		static HANDLE GetProcessByName(string name)	{
		    DWORD pid = 0;
		    name += ".exe";
		    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		    PROCESSENTRY32 process;
		    ZeroMemory(&process, sizeof(process));
		    process.dwSize = sizeof(process);

		    if (Process32First(snapshot, &process)) {
		        do {
		            cout<<process.szExeFile<<endl;
		            if (!strcmp(process.szExeFile, name.c_str())) {
		               pid = process.th32ProcessID;
		               break;
		            }
		        } while(Process32Next(snapshot, &process));
		    }

		    CloseHandle(snapshot);

		    if (pid != 0) {
		         return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		    }
		    return NULL;
		}
		static void waitEnd(Process* proc) {
			WaitForSingleObject(proc->hProcess, INFINITE);
			GetExitCodeProcess(proc->hProcess, &(proc->exitCode));
		}
	};
#elif defined(__linux__)
	#include "debug.hpp"
	#define HPROCESS int
	class Process {
	private:
		int exitCode;
		HPROCESS hProcess;
		string nameCmdLine;
		Process(HPROCESS hProcess) {
			this->hProcess = hProcess;
		}
	public:
		Process(string nameCmdLine, int startFlag = FALSE) {
			this->nameCmdLine = nameCmdLine;
			if(startFlag)
				this->start();
		}
		void start() {
			int pid = fork(); //create a fork
			if(pid == -1) // if error
				return;

			if(pid == 0) { //if son process, replace with exec
				int i = 0;
				char* commandLine = const_cast<char*>(nameCmdLine.c_str());
				while(commandLine[i++] != ' ');
				char* path = new char[i];
				i = 0;
				while(commandLine[i] != ' '){
					path[i] = commandLine[i++];
				};
				path[i] = '\0';
				while(commandLine[i] == ' ') {
					++i;
				}
				char* argv = (commandLine + i);
				execl(path," ", argv, NULL);
				hProcess = getpid();
			}
		}
		void stop() {
			kill(hProcess, SIGKILL);
		}
		static Process* takeExisting(string name) {
			HPROCESS hProcess = GetProcessByName(name);
			return !hProcess ? NULL : new Process(hProcess);
		}
		static HPROCESS GetProcessByName(string name) {
			char line[255];
			string pidof = "pidof " + name;
			FILE* cmd = popen(pidof.c_str(), "r");
			fgets(line, 255, cmd);
			pclose(cmd);
			return strtoul(line, NULL, 10);
		}
		static void waitEnd(Process* proc) {
			Debug::debug(proc->exitCode);
			int w = waitpid(proc->hProcess, &(proc->exitCode), WCONTINUED);
			Debug::debug(w);
			Debug::error(errno," waitpid error - ");
		}
	};
#else 
	#error unsupported system
#endif