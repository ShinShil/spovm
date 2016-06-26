#include "main.h"

int main(int argc, char const *argv[])
{
	string arg = "HEY!!!";
	for(int i = 0; i<arg.length(); ++i) {
		cout<<arg[i]<<endl;
	}
	cout<<"Process has been started.\nPress any key to  continue...";
	getchar();
	cout<<endl<<endl;
	return 0;
}