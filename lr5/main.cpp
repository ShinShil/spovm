#include "main.h"
#include "dllLoader.hpp"
#include "thread.hpp"
#include "debug.hpp"
#include "mutex.hpp"
#include "service.hpp"

#define FILE_INPUT_AMOUNT 3
Mutex mutexAsyncIO;
string inputFiles[FILE_INPUT_AMOUNT] = {"one.txt", "two.txt", "three.txt"};
string outFile = "out.txt";
string buff = "test";

string (*_read) (string path);
void (*_write)(string path, string src);
void (*_test)();

THREAD reader(PARAMS_THREAD_TYPE);
THREAD writer(PARAMS_THREAD_TYPE);
HDLL loadFunctions();

int main() {
	HDLL dll = loadFunctions();
	if(!dll) {
		return 0;
	}
	Debug::info("dll has loaded");
	//read - reads to buff
	//write - append buff
	for(int i = 0; i<FILE_INPUT_AMOUNT; ++i) {
		Thread threadReader(reader, Thread::packThreadData(inputFiles[i]));
		threadReader.startSync();
		Thread threadWriter(writer, Thread::packThreadData(outFile));
		threadWriter.startSync();
	}
	DllLoader::free(dll);
	cout<<endl;
	return 0;
}

THREAD reader(PARAMS_THREAD_TYPE args) {
	mutexAsyncIO.wait();
	Debug::info("start read");
	string path = Thread::unpackThreadData(args);
	Debug::info(path);
	buff = _read(path);
	Debug::info("end read");
	mutexAsyncIO.unlock();
}

THREAD writer(PARAMS_THREAD_TYPE args) {
	mutexAsyncIO.wait();
	Debug::info("start write");
	string path = Thread::unpackThreadData(args);
	_write(path, buff);
	mutexAsyncIO.unlock();
}

HDLL loadFunctions() {
	HDLL dll = DllLoader::load("dll\\lib_dll");
	if(!dll)  {
		cout<<"fail to load dll";
		return 0;
    }
	_test = (void (*)())DllLoader::getFunc(dll, "test");
	_read = (string (*)(string))DllLoader::getFunc(dll, "read");
	_write = (void (*)(string,string))DllLoader::getFunc(dll,"write");
	if(!_test) {
		cout<<"fail to load func: test";
		return 0;
	}
	if(!_read) {
		cout<<"fail to load func: read";
		return 0;
	}
	if(!_write) {
		cout<<"fail to load func: write";
		return 0;
	}
	return dll;	
}