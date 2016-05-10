#include "MainHeader.h"
#include "Service.h"
#include "ns_thread.h"
#include "FileAIO.h"

int main() {
	FileAIO::WriteFile("test");

	cout<<endl;
	return 0;
}