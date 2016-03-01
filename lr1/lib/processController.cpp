#include "processController.h"

Process::Process(TCHAR* path) {
	this->path = path;
#ifdef WIN32
	si = {sizeof(si)};
#endif
}

bool Process::start(TCHAR* commandLine) {
#ifdef WIN32
	return CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
#endif

#ifdef __linux__
	int status;
	int pid = fork(); //create a fork
	cout<<"Process hass been forked"<<endl;
	if(pid == -1) // if error
		return false;

	if(pid == 0) { //if son process, replace with exec
		int i = 0;
		while(commandLine[i++] != ' '){};
		TCHAR* path = new TCHAR[i];

		i = 0;
		while(commandLine[i] != ' '){
			path[i] = commandLine[i++];
		};
		path[i] = '\0';
		while(commandLine[i] == ' ') {
			++i;
		}

		TCHAR* argv = (commandLine + i);
		//cout<<"argv: "<<argv<<endl;
		//cout<<pid<<endl<<path<<endl<<argv<<endl;
		execl(path," ", argv, NULL);
		return false;
	}else {
		wait(&status);
		cout<<pid<<endl;
		return true;
	}

#endif
}

int Process::waitEnd() {
#ifdef WIN32
	return WaitForSingleObject(pi.hProcess, INFINITE);
#endif
#ifdef __linux__
	return 0;
#endif
}