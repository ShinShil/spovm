#pragma once
#include "main.h"
#include "fileObject.hpp"
//!!! IN THIS HEADER TWO CLASSES circular reference
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
	void cdDir(string dir);
	void cdUp();
	void start();

	void setNewSystem(string newSystem);

	string getSystem();

	FileObject* getFileObject();
};

#define SHUTDOWN 997
#define DEFAULT 0

#define ERRORS_AMOUNT 5
#define DUPLICATE_NAMES 0
#define NOT_FOUND_COMMAND 1
#define NOT_FOUND_FILE 2
#define NOT_FOUND_PATH 3
#define NOT_FOUND_DIR 4

#define COMMANDS_AMOUNT 12
#define MKDIR 0
#define EXIT 1
#define LS 2
#define LSDIR 3
#define MKFILE 4
#define CD 5
#define EDIT 6
#define SHOW 7
#define REMDIR 8
#define REMFILE 9
#define LSFILE 10
#define HELP 11

class FileCommands {
private:
	static const string commands[COMMANDS_AMOUNT];
	static const string errors[ERRORS_AMOUNT];
	static const string openDelim;
	static const string closeDelim;
	static const string fileDelim;

	void mkdir(FileSystemUser* fileSystem, string dir);
	void mkfile(FileSystemUser* fileSystem, string dir);
	void remdir(FileSystemUser* fileSystem, string dir);
	void remfile(FileSystemUser* fileSystem, string dir);
	void cd(FileSystemUser* fileSystem, string dir);
	void ls(FileSystemUser* fileSystem);
	void lsdir(FileSystemUser* fileSystem);
	void lsfile(FileSystemUser* fileSystem);
	void edit(FileSystemUser* fileSystem, string dir);
	void show(FileSystemUser* fileSystem, string dir);
	void duplicateNames(string dir);
	void notFoundCommand(string command);
	void notFoundDir(string dir);
	void notFoundFile(string file);
	void notFoundPath(string path);
	void help();
public:
	int analyze(string command, FileSystemUser* fileSystem);
};

#endif