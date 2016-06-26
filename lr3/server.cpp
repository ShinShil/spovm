#include "main.h"
#include "process.hpp"
#include "pipe.hpp"
#include "semaphore.hpp"

int main() {
	string message = "";
	Semaphore semaServer("serverSema");
	Semaphore semaMessage("messageSema");
	semaMessage.create();
	Pipe pipe("lab3");
	pipe.create();
	semaServer.open();
	semaServer.signal();
	while(message != "exit") {
		semaMessage.wait();
		cout<<endl<<"[Server] Enter message for client: ";
		cin>>message;
		pipe.write(message);
		cout<<endl<<"[Server] Waiting for client read";
		semaMessage.signal();
	}
	cout<<endl;
	return 0;
}