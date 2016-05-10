#include "MainHeader.h"
#include "ns_thread.h"
#include "Service.h"

char strings[10][20] = {
		"first thread\n\0",
		"second thread\n\0",
		"third thread\n\0",
		"fourth thread\n\0",
		"fifth thread\n\0",
		"sixth thread\n\0",
		"seventh thread\n\0",
		"eighth thread\n\0",
		"nineth thread\n\0",
		"tenth thread\n\0"
	};

int nextAddingThread = 0;
vector<Ns_thread*> threads;
int pauseFlag = 0;
int currThread = -1;
int exitFlag = 0;

THREAD_START_ROUTINE_TYPE printOneThread(THREAD_START_ROUTINE_ARGSTYPE args) {
	char* str = new char[512];
	str = (char*)args;
	//cout<<"Pause flag one: "<<pauseFlag<<endl;

	while(!exitFlag) {
		if(pauseFlag) {
			int i = 0;
			//cout<<"In loop: "<< str[0] << " Pause flag: " << pauseFlag <<endl;
			while(str[i]!='\0') {
				cout<<str[i++];
				sleep(1);
			}
			//cout<<"Pre pause = 0"<<endl;
			pauseFlag = 0;
		}
	}
	return 0;
}

THREAD_START_ROUTINE_TYPE printerThread(THREAD_START_ROUTINE_ARGSTYPE args) {
	while(!exitFlag) {
		//cout<<"Print: "<<exitFlag<<endl;
		//cout<<"Size: "<<threads.size()<<endl;
		if(threads.size() > 0) {
			pauseFlag = 1;
			//cout<<"Pause flag printer: "<<pauseFlag<<endl;
			threads[currThread]->create((THREAD_START_ROUTINE_ARGSTYPE)strings[currThread], printOneThread);
			while(pauseFlag);
			threads[currThread]->destroy();
			Service::cycleIncrement(currThread, threads.size());
		}
	}	
	return 0;
}

THREAD_START_ROUTINE_TYPE inputThread(THREAD_START_ROUTINE_ARGSTYPE args) {
	char ch;
		cout<<"wait for input: ";
	while(TRUE) {
		cin>>ch;
		switch(ch) {
			case '+': {
				Ns_thread* thread = new Ns_thread();
				if(!nextAddingThread)
					currThread = 0;
				++nextAddingThread;
				threads.push_back(thread);
				break;
			}
			case '-':{
				--nextAddingThread;
				threads.pop_back();
				break;
			}
			case 'q': {
				exitFlag = 1;
				if(nextAddingThread)
					while(pauseFlag);			
				while(threads.size()) {
					threads[threads.size() - 1]->destroy();
					threads.pop_back();
				}
				return 0;
			}
		}
	}
}

int main() {

	Ns_thread printer, input;
	printer.create(NULL, printerThread);
	input.create(NULL, inputThread);
	input.wait();
	input.destroy();
	printer.destroy();
	cout<<endl;
	return 0;
}