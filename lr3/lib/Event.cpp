#include "Event.h"

/*
создаём объект Event
в нём есть переменная булева state
ф-я wait()
{
	while(state);
}
a-z set() {
	state = true;
}
ф-я reset() {
	state = false;
}

в конутруктор задать реакцию на приходящий сигнал, использовать два сигнала SIGUSR1 и SIGUSR2 для set и reset
добавить в ф-ю set Kill(pid)
т.е. в конструктор для линукса должен приходить массив pid процессов, которые будут подписаны на событие - лучше добавить ф-ю setPid, которая будет иметь пустую ре-ю на win и ствить pid на линуксе
нужны дополнительные методы setBySignal, resetBySignal - чтобы установка state не вызывала бесконечной рассылки
*/
vector<string> Event::names;
vector<int> Event::states;
string Event::pipePath;
int Event::setSignals = FALSE;

void setBySignal(int signo) {
	string _name;
	Service::readFromFile(_name, Event::pipePath);
	int ind = Event::getIndexName(_name);
	if(ind  == -1) {
		return;
	}
	Event::states[ind] = TRUE;
}

void resetBySignal(int signo) {
	string _name;
	Service::readFromFile(_name, Event::pipePath);
	int ind = Event::getIndexName(_name);
	if(ind  == -1) {
		return;
	}
	Event::states[ind] = FALSE;
}

int Event::getIndexName(string name) {
	//cout<<"Size: "<<Event::names.size()<<endl;

	for(int i = 0; i<Event::names.size(); ++i) {
		if(name == names[i])
			return i;
	}
	return -1;
}

Event::Event(string name) {
#ifdef WIN32
	hEvent = CreateEvent(NULL, TRUE, FALSE, name.c_str());
	this->name = name;
#endif
#ifdef __linux__
	Event::pipePath = "pipeEvent.txt";
	subscribers = new vector<HEVENT>();
	if(!setSignals) {
		struct sigaction usr1;
		memset(&usr1, 0, sizeof(usr1));
		usr1.sa_handler = setBySignal;
		sigset_t set;
		sigemptyset(&set);
		sigaddset(&set, SIGUSR1);
		usr1.sa_mask = set;
		sigaction(SIGUSR1, &usr1, 0);

		struct sigaction usr2;
		memset(&usr2, 0, sizeof(usr2));
		usr2.sa_handler = resetBySignal;
		sigset_t set2;
		sigemptyset(&set2);
		sigaddset(&set2, SIGUSR2);
		usr2.sa_mask = set2;
		sigaction(SIGUSR2, &usr2, 0);
		setSignals = TRUE;
	}
	hEvent = getpid();
	this->name = name;
	cout<<"-2"<<endl;
	int ind = Event::getIndexName(name);
	if(ind == -1) {
		states.push_back(false);
		names.push_back(name);
	}
	cout<<"sigs are setted, pid = "<<hEvent<<endl;
#endif	
}

Event::Event(HEVENT hEvent) {
	this->hEvent = hEvent;//create signal
}

HEVENT Event::getHEvent() {
	return hEvent;
}

Event* Event::open(string name) {
#ifdef WIN32
	HEVENT hEvent = OpenEvent(EVENT_ALL_ACCESS|EVENT_MODIFY_STATE, FALSE, name.c_str());
	return new Event(hEvent);
#endif
#ifdef ___linux__
	return new Event(name);
#endif
}

void Event::set() {
#ifdef WIN32
	SetEvent(this->hEvent);
#endif
#ifdef __linux__
	 //послыка массиву pid сигнала USR1
	cout<<"SET"<<endl;
	for(int i = 0; i<subscribers.size(); ++i) {
		kill(subscribers[i], SIGUSR1);
	}
	Service::writeToFile(name, pipePath);
	states[Event::getIndexName(name)] = TRUE;	
#endif
}

void Event::reset() {
#ifdef WIN32
	ResetEvent(this->hEvent);
#endif
#ifdef __linux__
	//посылка массиву pid сигнала USR2
	for(int i = 0; i<subscribers.size(); ++i) {
		kill(subscribers[i], SIGUSR2);
	}
	Service::writeToFile(name, pipePath);
	states[Event::getIndexName(name)] = FALSE;	
#endif
}

void Event::wait() {
#ifdef WIN32
	WaitForSingleObject(hEvent, INFINITE);
#endif
#ifdef __linux__
	cout<<getpid()<<" process is waiting";
	while(!states[Event::getIndexName(name)]);
#endif
}


void Event::setSubscribers(vector<HEVENT>& subscribers) {
	this->subscribers = subscribers;
}

void Event::addSubscriber(HEVENT subscriber) {
	cout<<"sub: "<<subscriber<<endl;
	this->subscribers->push_back(subscriber);
	cout<<"added subs";
}