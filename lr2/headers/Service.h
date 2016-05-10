#pragma once
#include "MainHeader.h"

class Service {
public:
	static void writeToFile(string src, string path);
	static void readFromFile(string& dest, string path);
	static string getCurrDir();
	static void cycleIncrement(int& i, int max);
};