#pragma once
#include "main.h"
#include "stringService.hpp"
#include "fileSystemUser.h"
#include "debug.hpp"

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

