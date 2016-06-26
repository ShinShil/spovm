#include "main.h"
#include "thread.hpp"
#include "mutex.hpp"
#include "service.hpp"

int printDelay = 200;

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

vector<Thread*> printers;
vector<Thread*> actions; //add/remove, after input(), action saves, than wait for end of current thread execution and acts all of them 
int activePrinter = -1;
int infoPrinterLoop = 0;

THREAD addPrinter(void* params);
THREAD removePrinter(void* params);
THREAD printerThread(void* params);
THREAD printerLoop(void* params);
THREAD input(void* params);

Mutex mutex;
Mutex addActionsMutex;
int main() {
	Thread printer(printerLoop);
	printer.startAsync();
	while(true) {
		input(NULL);
	}
	cout<<"heeey"<<endl;
	cout<<endl;
	return 0;
}


THREAD addPrinter(void* params) {
	//cout<<"add"<<endl;
	printers.push_back(new Thread(printerThread, (void*)(Thread::packThreadData(strings[printers.size()]))));
	return 0;
}

THREAD removePrinter(void* params) {
	//cout<<"remove"<<endl;
	if(printers.size() > 0) 
		printers.pop_back();
	return 0;
}

THREAD printerThread(void* params) {
	mutex.wait();
	string str = Thread::unpackThreadData(params);
	for(int i = 0; i<str.length(); ++i) {
		cout<<str[i];
		sleep(1);
	}
	
	mutex.unlock();
	return 0;
}


//mutex - не даёт одновременно выполнять print и actions
THREAD printerLoop(void* params) {
	while(true) {

		if(activePrinter < 0) {
			if(!infoPrinterLoop) {
				cout<<"[info] Waiting for input"<<endl;
				infoPrinterLoop = 1;
			}
			if(actions.size()>0) {
				mutex.wait();
				for(int i = actions.size() - 1; i>=0; --i) {
					actions[i]->startSync();
					actions.pop_back();
				}
				if(printers.size() > 0)
					activePrinter = 0;
				mutex.unlock();
			}
		}else {
			infoPrinterLoop = 0;
			printers[activePrinter]->startSync();
			if(actions.size() > 0) {
				mutex.wait();
				for(int i = 0; i<actions.size(); ++i) {
					actions[i]->startSync();
					actions.pop_back();
				}
				mutex.unlock();
			}
			if(printers.size() > 0)
				Service::loopIncrement(activePrinter, printers.size());
			else {
				activePrinter = -1;
				infoPrinterLoop = 0;
			}
		}
	}
	return 0;
}

//input can't executed while actions not ended;
THREAD input(void* params) {
	if(actions.size() > 0) {
		mutex.wait();
		cout<<"[info] Waiting for actions"<<endl;
		mutex.unlock();
	}
	while(actions.size() > 0);
	string str;
	cin >> str;
	reverse(str.begin(), str.end());
	
	for(int i = 0; i<str.length(); ++i) {
		if(str[i] != '+' && str[i] != '-') {
			return 0;
		}
	}
	for(int i = 0; i<str.length(); ++i) {
		if(str[i] == '+') {
			if(printers.size() + 1 < 10) {
				actions.push_back(new Thread(addPrinter));
			} 
		} 
		if(str[i] == '-') {
				actions.push_back(new Thread(removePrinter));
		}
	}
	cout<<"actions hass been written"<<endl;
	
	return 0; 
}