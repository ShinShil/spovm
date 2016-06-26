#include "main.h"
#include "process.hpp"

int main() {
	Process* process = NULL;
	while(!process) {
		cout<<endl<<"Waiting for second process.\nPress any key";
		getchar();
		getchar();
		process = Process::takeExisting("process");
	}
	Process::waitEnd(process);
	cout<<endl<<"second process ended";
	cout<<endl<<endl;
	return 0;
}