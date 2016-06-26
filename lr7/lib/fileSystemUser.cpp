#include "fileSystemUser.h"
/*
string objectFileTypes[2] = {
	"file", "directory"
};

string commands[2] = {
	"mkdir",
	"exit"
};

string errors[2] = {
	"duplicate names - ",
	"not found such command - "
};
*/
const string FileCommands::commands[2]  = {
	"mkdir",
	"exit"
};
const string FileCommands::errors[2]  = {
	"duplicate names - ",
	"not found such command - "
};
FileSystemUser::FileSystemUser(string drive) {
	this->drive = drive;
	this->fileObject = new FileObject(getSystem(), "root", 5, '{', '}');
	this->currPath = drive + ":";
	this->currPath += "\\root";
	this->currPath += "\\";
}
string FileSystemUser::getCurrPath() {

}
string FileSystemUser::getCommand() {

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