#include "MainHeader.h"
#include "processController.h"
#include "systemFunc.h"

int main(int argc, TCHAR* argv[]) {
	if (argc > 1) {
		/*
		TCHAR* processName = "\\process.exe";
		TCHAR* dir = SystemFunc::getCurrDir();
		TCHAR* dir2 = new TCHAR[255];
		getcwd(dir2, 255);
		cout<<processName<<endl<<dir<<endl<<dir2<<endl<<processPath<<endl;
		cout<<"Process NOT forked"<<endl;
		return 0;
		*/
		TCHAR* processPath = SystemFunc::concat(SystemFunc::getCurrDir(), TEXT("/process "));
		TCHAR* commandLine = SystemFunc::concat(processPath, argv[1]);
		Process pr(processPath);
		bool bOk = pr.start(commandLine);
		if(bOk) {
			cout<<"Parent process"<<endl;
			pr.waitEnd();
			TCHAR* res = SystemFunc::readToCharFromFile(SystemFunc::concat(SystemFunc::getCurrDir(), TEXT("/pipe.txt")));
			cout << argv[1] << " * " << argv[1] << " = " << res<<endl;
		}else {
			cout<<"Fail to start the another process"<<endl;
		}
	}else {
		cout<<"You should enter the digit as parameter"<<endl;
	}
	return 0;
}