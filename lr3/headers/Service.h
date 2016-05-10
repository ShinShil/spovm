#pragma once
#include "MainHeader.h"

class Service {
private:
	static string debugName;
public:
	static void writeToFile(string src, string path);
	static void readFromFile(string& dest, string path);
	static string getCurrDir();
	static void cycleIncrement(int& i, int max);
	static void setDebugName(string str);
	static void debug(string str, string name = "");
	static void printLastError(string message);
};