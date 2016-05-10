#pragma once
#include "MainHeader.h"
#include "Service.h"

#ifdef WIN32
	#define HEVENT HANDLE
#endif
#ifdef __linux__
	#define HEVENT int
#endif


class Event;

class Event {
private:
	static int setSignals;
	HEVENT hEvent;
	string name;
	int ownName;
	int currState;
	int getName;
	vector*<HEVENT> subscribers;
public:
	static string pipePath;
	static vector<int> states;
	static vector<string> names;
	Event(string name);
	Event(HEVENT hEvent);
	static Event* open(string name);
	static int getIndexName(string name);
	void set();
	void reset();
	void wait();
	HEVENT getHEvent() ;
	void setSubscribers(vector<HEVENT>& subscribers);
	void addSubscriber(HEVENT subscriber);
};

void setBySignal(int signo);
void resetBySignal(int signo);