#include "MainHeader.h"
#include "Service.h"
#include "Event.h"
#include "Process.h"
int main() {
	char ch;
	Event* event = Event::open("event");
	cout<<"Input has been created"<<endl;
	event->addSubscriber(Process::getHandle("main"));
	while(true) {
		cout<<"loop"<<endl;
		cin>>ch;
		cin.clear();
		string str = "";
		str += ch;
		Service::writeToFile(str, "event.txt");
		//HANDLE event = OpenEvent(EVENT_ALL_ACCESS|EVENT_MODIFY_STATE, FALSE, "event");
		event->set();
		event->wait();
		cout<<"succefull waiting"<<endl;
	}
	return 0;
}