#include "fileSystemUser.h"

const string FileCommands::commands[COMMANDS_AMOUNT]  = {
	"mkdir",
	"exit",
	"ls",
	"lsdir",
	"mkfile",
	"cd",
	"edit",
	"show",
	"remdir",
	"remfile",
	"lsfile",
	"help"
};
const string FileCommands::errors[ERRORS_AMOUNT]  = {
	"duplicate names - ",
	"not found such command - ",
	"not founf such file - ",
	"not found such path - ",
	"not found such dir - "
};

const string FileCommands::openDelim = "{";
const string FileCommands::closeDelim = "}";
const string FileCommands::fileDelim = "$";

void FileCommands::mkdir(FileSystemUser* fileSystem, string dir) {
	FileObject* fo = fileSystem->getFileObject();
	if(fo->containsObject(dir)) {
		duplicateNames(dir);
		return;
	}
	
	dir += "{";
	dir += "}";
	fo->setEndPos();
	string sys = fileSystem->getSystem();
	sys.insert(fo->getPos(), dir);
	fileSystem->setNewSystem(sys);
	fo->addObj(dir);
}
void FileCommands::mkfile(FileSystemUser* fileSystem, string file) {
	FileObject* fo = fileSystem->getFileObject();
	if(fo->containsObject(file)) {
		duplicateNames(file);
		return;
	}
	file += fileDelim;
	file += openDelim;
	file += closeDelim;
	fo->setEndPos();
	string sys = fileSystem->getSystem();
	sys.insert(fo->getPos(), file);
	fileSystem->setNewSystem(sys);
	fo->addObj(file);
}
void FileCommands::remdir(FileSystemUser* fileSystem, string dir) {
	FileObject* fo = fileSystem->getFileObject();
	if(!fo->containsObject(dir)) {
		notFoundDir(dir);
		return;
	}
	string sys = fileSystem->getSystem();
	int t = fo->getObject(dir);
	cout<<"t: "<<t<<" pos:"<<fo->getPos()<<endl;
	if(fo->getPos() == fo->getBeginPos()) {
		fo->setEndPos();
	}
	sys.erase(t, fo->getPos() - t);
	fileSystem->setNewSystem(sys);	
	fo->remObj(dir);
}
void FileCommands::remfile(FileSystemUser* fileSystem, string file) {
	FileObject* fo = fileSystem->getFileObject();
	if(!fo->containsObject(file)) {
		notFoundFile(file);
		return;
	}
	string sys = fileSystem->getSystem();
	int t = fo->getObject(file);
	sys.erase(t, fo->getPos() - t - 1);
	fileSystem->setNewSystem(sys);
	fo->remObj(file);	
}
void FileCommands::cd(FileSystemUser* fileSystem, string path) {
	FileObject* fo = fileSystem->getFileObject();
	vector<string> dirs = StringService::pathSplit(path);
	int oldPos = fo->getPos();
	for(int i = 0; i<dirs.size(); ++i) {
		if(dirs[i]!="..") {
			if(!fo->containsObject(dirs[i])) {
				notFoundPath(path);
				fo->setPos(oldPos);
				return;		
			}else {
				fo->go(dirs[i]);
			}
		}else {
			fo->goUp();
		}
	}
	for(int i = 0; i<dirs.size(); ++i) {
		if(dirs[i] == "..") {
			fileSystem->cdUp();
		}else {
			fileSystem->cdDir(dirs[i]);
		}
	}
}
void FileCommands::ls(FileSystemUser* fileSystem) {
	FileObject* fo = fileSystem->getFileObject();
	cout<<endl;
	vector<string> fileObjects = fo->getObjects();
	for(int i = 0; i<fileObjects.size(); ++i) {
		cout<<fileObjects[i]<<endl;
	}	
	cout<<endl;
}
void FileCommands::lsdir(FileSystemUser* fileSystem) {
	FileObject* fo = fileSystem->getFileObject();
	cout<<endl;
	vector<string> dirs = fo->getDirs();
	for(int i = 0; i<dirs.size(); ++i) {
		cout<<dirs[i]<<endl;
	}	
	cout<<endl;
}
void FileCommands::lsfile(FileSystemUser* fileSystem) {
	FileObject* fo = fileSystem->getFileObject();
	cout<<endl;
	vector<string> files = fo->getFiles();
	for(int i = 0; i<files.size(); ++i) {
		cout<<files[i]<<endl;
	}	
	cout<<endl;
}
void FileCommands::edit(FileSystemUser* fileSystem, string fileName) {
	FileObject* fo = fileSystem->getFileObject();
	fileName += "$";
	int t = fo->getObject(fileName);
	if(t) {
		fo->setPos(t);
		string newContent;
		char buff[512];
		cout<<"enter new content: ";
		cin.getline(buff, 512);
		newContent = buff;
		int begPos = fo->getBeginPos();
		string sys = fileSystem->getSystem();
		fo->go(fileName);
		int beg = fo->getPos();
		fo->setEndPos();
		int end = fo->getPos();
		sys.erase(beg, end-beg + 1);
		sys.insert(beg, newContent);
		fileSystem->setNewSystem(sys);
		fo->setBeginPos(begPos);
		fo->setFileContent(fileName, newContent);
	}else {
		notFoundFile(fileName);
		return;
	}
	
}
void FileCommands::show(FileSystemUser* fileSystem, string fileName) {
	FileObject* fo = fileSystem->getFileObject();
	fileName += fileDelim;
	
	if(fo->containsObject(fileName)) {
		string content = fo->getFileContent(fileName);
		cout<<endl<<"\t"<<fileName<<endl<<endl<<content<<endl;
	}else {
		notFoundFile(fileName);
		return;
	}
}

void FileCommands::help() {
	cout<<endl;
	for(int i = 0; i<COMMANDS_AMOUNT; ++i) {
		cout<<commands[i]<<endl;
	}
	cout<<endl;
}

void FileCommands::notFoundDir(string dir) {
	string s = errors[NOT_FOUND_DIR];
	s += dir;
	Debug::error(s);
}

void FileCommands::notFoundFile(string file) {
	string s = errors[NOT_FOUND_FILE];
	s += file;
	Debug::error(s);
}

void FileCommands::notFoundPath(string path) {
	string s = errors[NOT_FOUND_PATH];
	s += path;
	Debug::error(s);
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
	if(args[0] == commands[MKFILE]) {
		mkfile(fileSystem, args[1]);
		return DEFAULT;
	}
	if(args[0] == commands[REMDIR]) {
		remdir(fileSystem, args[1]);
		return DEFAULT;
	}
	if(args[0] == commands[REMFILE]) {
		remfile(fileSystem, args[1]);
		return DEFAULT;
	}
	if(args[0] == commands[CD]) {
		cd(fileSystem, args[1]);
		return DEFAULT;
	}
	if(args[0] == commands[LS]) {
		ls(fileSystem);
		return DEFAULT;
	}
	if(args[0] == commands[LSDIR]) {
		lsdir(fileSystem);
		return DEFAULT;
	}
	if(args[0] == commands[LSFILE]) {
		lsfile(fileSystem);
		return DEFAULT;
	}
	if(args[0] == commands[EDIT]) {
		edit(fileSystem, args[1]);
		return DEFAULT;
	}
	if(args[0] == commands[SHOW]) {
		show(fileSystem, args[1]);
		return DEFAULT;
	}
	if(args[0] == commands[HELP]) {
		help();
		return DEFAULT;
	}

	notFoundCommand(args[0]);
	return DEFAULT;
}