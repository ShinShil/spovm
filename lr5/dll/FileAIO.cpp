#include "FileAIO.h"

	#if defined(WIN32)
		#define MAX_READ 512

		string FileAIO::readFileA(char* path, char* buffer) {
			HANDLE hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			
			int readenByte;
			buffer = new char[512];

			HANDLER hEvent = CreateEvent(NULL, FALSE, FALSE, "event");
			OVERLAPPED overlap;
			overlap.hEvent = hEvent;

			ReadFile(hFile, buffer, MAX_READ, &readenByte, &overlap);

			WaitForSingleObject(hEvent);
			string str = buffer;
			return string;

		}
		void FileAIO::writeFileA(char* path) {
			cout<<"Testing dll lib"<<endl;
		}
	#elif defined(__linux__)

	#else
		#error Unsupporting system
	#endif