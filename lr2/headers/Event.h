#pragma once
#include "MainHeader.h"
#include "Service.h"
#include "Process.h"
#ifdef WIN32
	#define HEVENT HANDLE
#endif
#ifdef __linux__
	#define HEVENT int
#endif

#ifdef WIN32
    class Event {
    private:
        HANDLE hEvent;
        string name;    
    public:
        Event(string name) {
            create(name);
        }
        void create(string name) {
            hEvent = CreateEvent(NULL, TRUE, FALSE, name.c_str());
            if(GetLastError() == ERROR_ALREADY_EXISTS)
                HEVENT hEvent = OpenEvent(EVENT_ALL_ACCESS|EVENT_MODIFY_STATE, FALSE, name.c_str());
        }
        void set() {
            SetEvent(hEvent);
        }
        void reset() {
            ResetEvent(hEvent);
        }
        void wait() {
            WaitForSingleObject(hEvent, INFINITE);
        }
    };
#endif


#ifdef __linux__
class SmallEvent {
private:
    string name;
    int state;
public:
    //1 - signal
    //0 - not signal
    SmallEvent(string name, int state = 0) {
        this->name = name;
        this->state = state;
    }
    void set() {
        state = 1;
    }
    void reset() {
        state = 0;
    }
    void wait() {
        while(!state);
    }
    string getName() {
        return name;
    }
    int getState() {
        return state;
    }
    void printInfo() {
        cout<<"eventSmal printInfo - "<<"Name: "<<name << "; State: "<<state<<endl;        
    }
};

class Event {
private:
    static vector<SmallEvent*> events;
    int eventSmallIndex;
    static int setSignals;
    string getSubsFileName(string eventName) {
        return "event" + eventName + "Subscribers.txt";
    }
    vector<string> getEventSubscribers(string name) {
        string subsFile = getSubsFileName(name);
        string subscribersLine = "";
        Service::readFromFile(subscribersLine, subsFile);
        vector<string> subscribers = Service::splitLines(subscribersLine);
        return subscribers;
    }
public:
    static void setSignal(int) {
        string name = "";
        Service::readLineFromFile(name, "tmp.txt");
        cout<<"Event setSignal: "<<name<<endl;
        for(int i = 0; i<events.size(); ++i) {
            cout<<(events[i]->getName().compare(name))<<"?"<<endl;
            if(!events[i]->getName().compare(name)) {
                events[i]->set();
            }
            events[i]->printInfo();
        }
    }
    static void resetSignal(int) {
        string name;
        cout<<"Event resetSignal"<<endl;
        Service::readFromFile(name, "tmp.txt");
        for(int i = 0; i<events.size(); ++i) {
            if(events[i]->getName() == name) {
                events[i]->reset();
            }
        }
    }
    Event(string name, int state = 0) {
        if(!setSignals) {
            struct sigaction usr1;
            memset(&usr1, 0, sizeof(usr1));
            usr1.sa_handler = setSignal;
            sigset_t set;
            sigemptyset(&set);
            sigaddset(&set, SIGUSR1);
            usr1.sa_mask = set;
            sigaction(SIGUSR1, &usr1, 0);

            struct sigaction usr2;
            memset(&usr2, 0, sizeof(usr2));
            usr2.sa_handler = resetSignal;
            sigset_t set2;
            sigemptyset(&set2);
            sigaddset(&set2, SIGUSR2);
            usr2.sa_mask = set2;
            sigaction(SIGUSR2, &usr2, 0);
            setSignals = TRUE;
        }
        create(name, state);
    }
    void create(string name, int state = 0) {
        int i = 0;
        //cout<<"Events size:"<<events.size()<<endl;
        HPROCESS hProcess = Process::getCurrProcessHandle();
        string strHProcess = Service::intToString(getpid());
        vector<string> subs = getEventSubscribers(name);
        if(subs.size() != 0) {
           // cout<<"Event open: "<<name<<endl;
            //if this event was opened previously
            for(i = 0; i < subs.size(); ++i) {
                if(subs[i] == strHProcess) {
                    for(int j = 0; j<events.size(); ++j) {
                        if(events[j]->getName() == name) {
                            eventSmallIndex = j;
                            return;
                        }
                    }
                }
            }
        }else {
         //   cout<<"Event create: "<<name<<endl;
        }
       // cout << "str: "<<strHProcess<<"; pid: "<<getpid()<<endl;
        Service::addLineToFile(strHProcess, getSubsFileName(name));
        events.push_back(new SmallEvent(name, state));
        eventSmallIndex = events.size() - 1;
    }
        
    
    void set() {
        string name = events[eventSmallIndex]->getName();
        cout<<"debug setEvent: "<<name<<endl;
        Service::writeToFile(name, "tmp.txt"); 
        vector<string> subscribers = getEventSubscribers(name);
        cout<<"Subscribers of "<<name<<endl;
        for(int i = 0; i<subscribers.size(); ++i) {
            cout<<"\t "<<subscribers[i]<<endl;
            int pid = Service::stringToInt(subscribers[i]);
            cout<<"\tpid: "<<pid<<endl;
            if(pid != getpid())
                kill(pid, SIGUSR1);            
        }
        events[eventSmallIndex]->set();
        events[eventSmallIndex]->printInfo();
    }
    
    void reset()  {
        string name = events[eventSmallIndex]->getName();
        Service::writeToFile(name, "tmp.txt"); 
        vector<string> subscribers = getEventSubscribers(name);
        for(int i = 0; i<subscribers.size(); ++i) {
            kill(Service::stringToInt(subscribers[i]), SIGUSR2);            
        }
        events[eventSmallIndex]->reset();
    }

    void wait() {
        events[eventSmallIndex]->wait();
    }
};
int Event::setSignals = FALSE;
vector<SmallEvent*> Event::events;
#endif