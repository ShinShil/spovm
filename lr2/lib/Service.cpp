#include "Service.h"

void Service::writeToFile(string src, string path) {
	ofstream out;
	out.open(path.c_str());
	out << src;
	out.close();
}
void Service::readFromFile(string& dest, string path) {
	ifstream in;
	in.open(path.c_str());
	in >> dest;
	in.close();
}
string Service::getCurrDir() {
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

void Service::cycleIncrement(int& i, int max) {
	if(i + 1 >= max) {
		i = 0;
	}else {
		++i;
	}
}