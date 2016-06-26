#pragma once
#include "main.h"
#include "stringService.hpp"
#include "fileSystemUser.hpp"
#include "debug.hpp"

#define SHUTDOWN 997
#define DEFAULT 0
string commands[2] = {
	"mkdir",
	"exit"
};

#define DUPLICATE_NAMES 0
#define NOT_FOUND_COMMAND 1
string errors[2] = {
	"duplicate names - ",
	"not found such command - "
};

#define MKDIR 0
#define EXIT 1
class FileCommands {
private:
	void mkdir(FileSystemUser* fileSystem, string dir) {
		FileObject* fo = fileSystem->getFileObject();
		if(fo->containsObject(dir)) {
			duplicateNames(dir);
			return;
		}
		dir = "," + dir;
		dir += "{}";
		fo->setEndPos();
		string sys = fileSystem->getSystem();
		sys.insert(fo->getPos(), dir);
		fileSystem->setNewSystem(sys);
	}
	void duplicateNames(string dir) {
		string s = errors[DUPLICATE_NAMES];
		s += dir;
		Debug::error(s);
	}
	void notFoundCommand(string command) {
		string s = errors[NOT_FOUND_COMMAND];
		s += command;
		Debug::error(s);
	}
public:
	int analyze(string command, FileSystemUser* fileSystem) {
		vector<string> args = StringService::commandSplit(command);
		if(args.size() == 0) {
			notFoundCommand(command);
			return DEFAULT;
		}
		
		if(args[0] == commands[MKDIR])  { 
			mkdir(fileSystem, args[1]);
			return DEFAULT;
		}
		if(args[0] == commands[EXIT]){ 
			return SHUTDOWN;
		}
	
		notFoundCommand(args[0]);
		return DEFAULT;
		
	}
};

