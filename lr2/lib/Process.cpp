#include "Process.h"

void Process::create(string cmdline) {
#ifdef WIN32
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	si.cb = sizeof(si);
	ZeroMemory(&si, sizeof(PROCESS_INFORMATION));
	char* tmp = new char[255];
	strcpy(tmp, cmdline.c_str());
	CreateProcess(NULL, tmp, NULL, NULL,FALSE,0,NULL,NULL,&si,&pi);
	hProcess = pi.hProcess;
	hMainThread = pi.hThread;
#endif
#ifdef __linux__ 
	int status;
	int pid = fork(); //create a fork
	if(pid == -1) // if error
		return;

	if(pid == 0) { //if son process, replace with exec
		int i = 0;
		char* commandLine = const_cast<char*>(cmdline.c_str());
		//cout<<cmdline<<endl;
		while(commandLine[i++] != ' '){};
		char* path = new char[i];
        cout<<"Son pid: "<<getpid();
		i = 0;
		while(commandLine[i] != ' '){
			path[i] = commandLine[i++];
		};
		path[i] = '\0';
		while(commandLine[i] == ' ') {
			++i;
		}
		char* argv = (commandLine + i);
		//cout<<"argv: "<<argv<<endl;
		//cout<<pid<<endl<<"PATH: " << path<<endl<<"ARGV: "<<argv<<endl;
		execl(path," ", argv, NULL);
		hProcess = getpid();
		cout<<"HEY"<<pid<<endl;
	}else {
		//wait(&status);
	}
#endif
	state  = RESUME;
}

void Process::close() {
#ifdef WIN32
	TerminateProcess(hProcess, NO_ERROR);
#endif
#ifdef __linux__
	kill(hProcess, SIGKILL);
#endif
}

void Process::pause() {
#ifdef WIN32
	SuspendThread(hMainThread);
#endif
#ifdef __linux__
    cout << name << " pause" <<endl;
	kill(hProcess, SIGTSTP);
#endif
	state = PAUSE;
	//cout<<endl<<name<<" pause"<<endl;
}

void Process::resume() {
#ifdef WIN32
	ResumeThread(hMainThread);
#endif
#ifdef __linux__
	kill(hProcess, SIGCONT);
#endif
	//cout<<endl<<name<<" resume"<<endl;
	state = RESUME;
}

int Process::getState() {
	return state;
}

void Process::waitSignal(HPROCESS hProcess, UINT time) {
#ifdef WIN32
	WaitForSingleObject(hProcess, time);
#endif
#ifdef __linux__
#endif	
}

HPROCESS Process::getHProcess() {
	return hProcess;
}

HPROCESS Process::getHandle(string name) {
#ifdef WIN32
	return NULL;
#endif
#ifdef __linux__ 
	char line[255];
	string pidof = "pidof " + name;
	FILE* cmd = popen(pidof.c_str(), "r");
	fgets(line, 255, cmd);
	pclose(cmd);
	return strtoul(line, NULL, 10);
#endif
}

HPROCESS Process::getCurrProcessHandle() {
#ifdef WIN32
    return NULL;
#endif

#ifdef __linux__
    return getpid();
#endif    
}
