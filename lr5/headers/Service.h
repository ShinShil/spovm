#pragma once
#include "MainHeader.h"

class Service {
private:
	static string debugName;
public:
	static void writeToFile(string src, string path) {
		ofstream out;
		out.open(path.c_str());
		out << src;
		out.close();
	}
	static void readFromFile(string& dest, string path) {
		ifstream in;
		in.open(path.c_str());
		in >> dest;
		in.close();
	}
	static string getCurrDir() {
		string currDir;
		char* tmp  = new char[255];
		#ifdef WIN32
			GetCurrentDirectory(255, tmp);
		#endif
		#ifdef __linux__
			getcwd(tmp, 255);
		#endif
			currDir = tmp;
			return currDir;
	}
	static void cycleIncrement(int& i, int max) {
		if(i + 1 >= max) {
			i = 0;
		}else {
			++i;
		}
	}
};