#pragma once
#include "main.h"
#include "fileObject.hpp"
#ifndef FILESYSTEMUSER 
#define FILESYSTEMUSER
class FileSystemUser {
private:
	string drive;//absolute path to .txt
	FileObject* fileObject;
	string currPath;
public:
	FileSystemUser(string drive);
	string getCurrPath();
	string getCommand();
	void start();

	void setNewSystem(string newSystem);

	string getSystem();

	FileObject* getFileObject();
};

#define SHUTDOWN 997
#define DEFAULT 0

#define DUPLICATE_NAMES 0
#define NOT_FOUND_COMMAND 1

#define MKDIR 0
#define EXIT 1

class FileCommands {
private:
	
	static const string commands[2];
	static const string errors[2];
	void mkdir(FileSystemUser* fileSystem, string dir);
	void duplicateNames(string dir);
	void notFoundCommand(string command);
public:
	int analyze(string command, FileSystemUser* fileSystem);
};

#endif