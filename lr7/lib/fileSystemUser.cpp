#include "fileSystemUser.h"


FileSystemUser::FileSystemUser(string drive) {
	this->drive = drive;
	this->fileObject = new FileObject(getSystem(), "root", 5, '{', '}');
	this->currPath = drive + ":";
	this->currPath += "\\root";
	this->currPath += "\\";
}
string FileSystemUser::getCurrPath() {
	return currPath;
}
string FileSystemUser::getCommand() {
 

}
void FileSystemUser::cdDir(string dir) {
	currPath += dir;
	currPath += "\\";
}
void FileSystemUser::cdUp() {
	int i = currPath.length() - 2;
	if(i<=drive.length() + 1 + 5) {
		return;
	}
	else {
		while(!StringService::isPathSeparator(currPath[i--]));
		currPath.erase(i+1, currPath.length() - i);
		currPath += "\\";
	}
}
void FileSystemUser::start() {
	FileCommands fc;
	int fcRes;
	while(true) {
		string command = "";
		cout << endl << currPath << " ";
		std::getline(cin, command);
		fcRes = fc.analyze(command, this);
		if(fcRes == SHUTDOWN) 
			break;
	}
}

void FileSystemUser::setNewSystem(string newSystem) {
	ofstream os;
	os.open(drive.c_str());
	os << newSystem;
	os.close();
}

string FileSystemUser::getSystem() {
	ifstream ifs;
	string content;
	cout<<drive<<endl;
	ifs.open(drive.c_str());
	while (ifs.good()) {
		char* tmp = new char[512];
		ifs.getline(tmp, 512);
		content += tmp;
	}
	ifs.close();
	return content;
}

FileObject* FileSystemUser::getFileObject() {
	return fileObject;
}