#include "MainHeader.h"
#include "Process.h"
#include "Semaphore.h"
#include "Pipe.h"
#include "Service.h"

int main() {
	Service::setDebugName("client");
	Service::debug("Waiting for server");
	string serverProcess = Service::getCurrDir() + "/server";
	Process server;

	while(1) {
		Pipe pipe;
		pipe.create("pipeName");
		Semaphore sem;
		sem.create("semName");
		server.create(serverProcess);
		sem.wait();

		string test;
		Service::debug("Reading the string");
		pipe.read(test);
		Service::debug(test);
		cout<<"-----------------"<<endl;
	}

	return 0;
}