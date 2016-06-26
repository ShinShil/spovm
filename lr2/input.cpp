#include "MainHeader.h"
#include "Service.h"
#include "Event.h"
int main() {
	char ch;
	Event* event = new Event("event");
	while(true) {
        cout<<"debug input"<<endl;
		cin>>ch;
		cin.clear();
		string str = "";
		str += ch;
		Service::writeToFile(str, "event.txt");
		event->set();
        cout<<"debug AFTER set event in input"<<endl;
	}
	return 0;
}