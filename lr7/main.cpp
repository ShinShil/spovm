#include "main.h"
#include "fileSystemUser.h"

int main() {
	FileSystemUser fc("fileSystemExample.txt");
	fc.start();
	return 0;
}