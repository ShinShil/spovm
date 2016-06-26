#include "main.h"

class Service {
public:
	static void hello() {
		cout<<endl<<"Hello, World!!!"<<endl;
	}
	static void loopIncrement(int& i, int max, int min = 0) {
		i + 1 >= max ? i=0 : i+=1;
	}	
	static void sleep(int ms) {
		#ifdef WIN32
			Sleep(ms);
		#else 
			sleep(ms/1000);
		#endif
	}
	static vector<string> split(string src) {
		vector<string> res;
		string tmp = "";
		for(int i = 0; i<src.length(); ++i) {
			if(src[i] == ' ') {
				res.push_back(tmp);
				tmp = "";
			}else {
				tmp += res[i];
			}
		}
		res.push_back(tmp);
		return res;
	}
};