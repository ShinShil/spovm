#include "MainHeader.h"
#include "Semaphore.h"
#include "Pipe.h"
#include "Service.h"

int main() {

	Service::setDebugName("server");
	Semaphore sem;
	Pipe pipe;
	Service::debug("The server has been launched");

	pipe.open("pipeName");
	

	string str;
	Service::debug("Waiting for the enter string");
	Service::debug("Enter the string");
	cin>>str;

	pipe.write(str);

	sem.open("semName");
	sem.signal();
	return 0;
}