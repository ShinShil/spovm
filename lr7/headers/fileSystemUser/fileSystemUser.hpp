#pragma once
#include "main.h"
#include "fileCommands.h"
#include "fileObject.hpp"


class FileSystemUser {
private:
	string drive;//absolute path to .txt
	FileObject* fileObject;
	string currPath;
public:
	FileSystemUser(string drive) {
		this->drive = drive;
		this->fileObject = new FileObject("root", 5, '{', '}');
		this->currPath = drive + ":";
		this->currPath += "\\root";
		this->currPath += "root";
		this->currPath += "\\";
	}
	string getCurrPath();
	string getCommand();
	void start() {
		FileCommands fc;
		int fcRes;
		while(true) {
			string command = "";
			cout << endl << currPath << " ";
			cin >> command;
			fcRes = fc.analyze(command, this);
			if(fcRes == SHUTDOWN) 
				break;
		}
	}

	void setNewSystem(string newSystem) {
		ofstream os;
		os.open(drive.c_str());
		os << newSystem;
		os.close();
	}

	string getSystem() {
		ifstream is;
		is.open(drive.c_str());
		string res;
		is >> res;
		is.close();
		return res;
	}

	FileObject* getFileObject() {
		return fileObject;
	}
};