#include "fileSystemUser.h"

void FileCommands::mkdir(FileSystemUser* fileSystem, string dir) {
	FileObject* fo = fileSystem->getFileObject();
	if(fo->containsObject(dir)) {
		duplicateNames(dir);
		return;
	}
	dir = dir;
	dir += "{}";
	fo->setEndPos();
	string sys = fileSystem->getSystem();
	sys.insert(fo->getPos(), dir);
	fileSystem->setNewSystem(sys);
}
void FileCommands::mkfile(FileSystemUser* fileSystem, string dir) {
	FileObject* fo = fileSystem->getFileObject();
	if(fo->containsObject(dir)) {
		duplicateNames(dir);
		return;
	}
	dir += "$";//fileMarker
	dir += "{}";
	fo->setEndPos();
	string sys = fileSystem->getSystem();
	sys.insert(fo->getPos(), dir);
	fileSystem->setNewSystem(sys);	
}
void FileCommands::cd(FileSystemUser* fileSystem, string path) {
	FileObject* fo = fileSystem->getFileObject();
	fo->cdPos(string path);
}
void FileCommands::ls(FileSystemUser* fileSystem) {
	
}
void FileCommands::lsdir(FileSystemUser* fileSystem) {

}
void FileCommands::lsfile(FileSystemUser* fileSystem) {

}
void FileCommands::edit(FileSystemUser* fileSystem, string fileName) {

}
void FileCommands::show(FileSystemUser* fileSystem, string fileName) {

}

void FileCommands::duplicateNames(string dir) {
	string s = errors[DUPLICATE_NAMES];
	s += dir;
	Debug::error(s);
}
void FileCommands::notFoundCommand(string command) {
	string s = errors[NOT_FOUND_COMMAND];
	s += command;
	Debug::error(s);
}
int FileCommands::analyze(string command, FileSystemUser* fileSystem) {
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