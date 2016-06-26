#include "MainHeader.h"
#include "Service.h"
#include "Process.h"
#include "Event.h"
int main(int argc, char* argv[]) {
    cout<<"debug printer"<<endl;
	string str = argv[1];
	str += " process";
	string loop = "loop";
	string pathToFile = Service::getCurrDir() + "\\event.txt";
	Event* event = new Event("event");
	Event* eventPrint = new Event("eventPrint");

	while(true) {
		//cout<<"waiting for eventPrint"<<endl;
		eventPrint->wait();
		for(int i = 0; i<str.length(); ++i) {
			cout<<str[i];
			Service::sleep(100);
		}
		cout<<endl;
		Service::writeToFile(loop, pathToFile);
		eventPrint->reset();
		//cout<<"eventPrint reset"<<endl;
		event->set();
	}
	cout<<endl;
	return 0;
}