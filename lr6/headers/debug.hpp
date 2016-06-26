#pragma once
#include "main.h"
#define DEBUG TRUE
class Debug {
public:
	static void debug(string line, string additional = "") {
		if(DEBUG)
		cout<<"[Debug] "<<additional<<line<<endl;
	}
	static void debug(int line, string additional = "") {
		if(DEBUG)
		cout<<"[Debug] "<<additional<<line<<endl;
	}
	static void info(string line) {
		if(DEBUG)
		cout<<"[Info] "<<line<<endl;
	}
	static void obj(string name, string line) {
		if(DEBUG)
			cout<<"["<<name<<"] "<<line<<endl;
	}
	static void error(string line, string additional = "") {
		cout<<"[error] "<<line<<endl;
	}
	static void error(int line, string additional = "") {
		cout<<"[error] "<<additional<<line<<endl;
	}
	static void message(string owner, string line) {
		if(DEBUG){
			string name = "[" + owner + "] " + line;
			cout<<line<<endl;
		}
	}
};