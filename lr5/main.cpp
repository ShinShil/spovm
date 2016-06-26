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

string (*read) (string path);
string buff = "test";
void (*write)(string path, string src);
void (*test)();

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
	buff = read(path);
	Debug::info("end read");
	mutexAsyncIO.unlock();
}

THREAD writer(PARAMS_THREAD_TYPE args) {
	mutexAsyncIO.wait();
	Debug::info("start write");
	string path = Thread::unpackThreadData(args);
	write(path, buff);
	mutexAsyncIO.unlock();
}

HDLL loadFunctions() {
	HDLL dll = DllLoader::load("dll\\lib_dll");
	if(!dll)  {
		cout<<"fail to load dll";
		return 0;
    }
	test = (void (*)())DllLoader::getFunc(dll, "test");
	read = (string (*)(string))DllLoader::getFunc(dll, "read");
	write = (void (*)(string,string))DllLoader::getFunc(dll,"write");
	if(!test) {
		cout<<"fail to load func: test";
		return 0;
	}
	if(!read) {
		cout<<"fail to load func: read";
		return 0;
	}
	if(!write) {
		cout<<"fail to load func: write";
		return 0;
	}
	return dll;	
}