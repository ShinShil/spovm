#include "MainHeader.h"
#include "systemFunc.h"

int main(int argc, TCHAR* argv[]) {
	if(argc > 1) {
		//cout<<"Hello from process"<<endl;
		double a;
		SystemFunc::FormDigit(0, argv[1], a);
		double res = a*a;
		TSTRING resStr;
		SystemFunc::DigitToStr(res, resStr);
		const char* str = resStr.c_str();
		SystemFunc::writeFromCharToFile(str, SystemFunc::concat(SystemFunc::getCurrDir(), TEXT("/pipe.txt")));
	}
	return 0;
}