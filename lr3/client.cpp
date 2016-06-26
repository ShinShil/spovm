#include "main.h"
#include "process.hpp"
#include "pipe.hpp"
#include "semaphore.hpp"

int main() {
	cout<<"[Client] Waiting for server launch"<<endl;
	Semaphore semaServer("serverSema");
	semaServer.create();
	semaServer.wait();
	Semaphore semaMessage("messageSema");
	semaMessage.open();
	Pipe pipe("lab3");
	pipe.connectClient();
	while(true) {
		semaMessage.signal();
		semaMessage.wait();
		string message = pipe.read();
		if(message == "exit")
			break;
		cout<<"[Client] "<<message<<endl;
	}
	cout<<"Server stopped"<<endl;
	return 0;
}