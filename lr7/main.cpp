#include "main.h"
#include "fileSystemUser.h"

int main() {
	cout<<"Print help"<<endl<<endl;
	FileSystemUser fc("fileSystemExample.txt");
	fc.start();
	return 0;
}