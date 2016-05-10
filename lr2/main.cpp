#include "MainHeader.h"
#include "Process.h"
#include "Service.h"
#include "Event.h"

int main() {
	cout<<"HI"<<endl;
	cout<<getpid()<<endl;
	cout<<Process::getHandle("main")<<endl;
	string testInput = Service::getCurrDir() + "/input ";
	Event* event2 = new Event("event");

	Process test;
	test.create(testInput);
	event2->wait();
	return 0;
	
	string cmdline[10] = { "first", "second", "third", "fourth", "fifth", "sixth", "seventh",  "eighth", "nineth", "tenth"}; 
	vector <Process*> prs;
	Process input;
	int loop = 1;
	int i = -1;
	string pathPrinter = Service::getCurrDir() + "\\printer";
	/*HANDLE event = CreateEvent(NULL, TRUE, FALSE, "event"); //create event with usual permesinons, manual, start state unsignaled, name 'event'
	HANDLE eventPrint = CreateEvent(NULL, TRUE, FALSE, "eventPrint"); //create event with usual permesinons, manual, start state unsignaled, name 'event'
	*/

	Event* event = new Event("event");
	Event* eventPrint = new Event("eventPrint");
	string pathInput = Service::getCurrDir() + "\\input";
/*
	Process::waitSignal(event, INFINITE);
	input.pause();
	string res;
	Service::readFromFile(res, "event.txt");
	cout<<"res1: "<<res<<endl;
	input.resume();
	Process::waitSignal(input.getHProcess(), INFINITE);
	Service::readFromFile(res, "event.txt");
	cout<<"res2: "<<res<<endl;
*/
	//use events, because according to the task printers should'nt terminated
	input.create(pathInput);
	input.name = "input";
	int allPause = 0;
	int maxI = 0;
	string tmp;
	string res;
	cout<<"Waiting for '+', '-', '/', '*', 'q'(quit)"<<endl;
	while(loop) {
		//cout<<endl<<"I: "<<i<<"; allPause: "<<allPause<<"; res: "<<res<<endl;
		if(i>=0 && !allPause) {
			eventPrint->set(); 
			prs[i]->resume();
		}
		event->wait();
		if(i>=0 && prs[i]->getState() == RESUME) {
			prs[i]->pause();
		}
		Service::readFromFile(res, "event.txt");
		input.pause();
		if(res == "loop") {
			Service::cycleIncrement(i, prs.size());
		}else {
			char op = res[0];
			switch(op) {
				case '+':
					prs.push_back(new Process());
					i = prs.size() - 1;
					tmp = pathPrinter + " " + cmdline[i];
					prs[i]->create(tmp);
					prs[i]->name = cmdline[i];
					if(allPause) {
						cout<<i;
						prs[i]->pause();
					}
					break;
				case '-': 
					if(i<0) {
						break;
					}
					if(prs.size() == 1) {
						prs.pop_back();
						--i;
						break;
					}

					prs.pop_back();
					Service::cycleIncrement(i, prs.size());
					if(!allPause){
						prs[i]->resume();
					}
					break;
				case '/':
					allPause = 1;
					break;
				case '*':
					allPause = 0;
					break;
				case 'q':
					if(!allPause && i>=0) {
						prs[i]->pause();
					}
					loop = 0;
					break;
				default:
					break;
			}
		}
		event->reset();
		input.resume();
	}
	for(int i = 0; i<prs.size(); ++i) {
		prs[i]->close();
	}
	cout<<endl;
	return 0;
}