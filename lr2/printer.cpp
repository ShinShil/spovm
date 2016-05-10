#include "MainHeader.h"
#include "Service.h"
#include "Process.h"
#include "Event.h"
int main(int argc, char* argv[]) {
	string str = argv[1];
	str += " process";
	string loop = "loop";
	string pathToFile = Service::getCurrDir() + "\\event.txt";
	/*HANDLE event = OpenEvent(EVENT_ALL_ACCESS|EVENT_MODIFY_STATE, FALSE, "event");
	HANDLE eventPrint = OpenEvent(EVENT_ALL_ACCESS|EVENT_MODIFY_STATE, FALSE, "eventPrint");*/
	Event* event = Event::open("event");
	Event* eventPrint = Event::open("eventPrint");

	while(true) {
		//cout<<"waiting for eventPrint"<<endl;
		eventPrint->wait();
		for(int i = 0; i<str.length(); ++i) {
			cout<<str[i];
			sleep(100);
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