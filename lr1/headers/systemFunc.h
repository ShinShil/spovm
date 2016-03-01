#pragma once
#include "MainHeader.h"

class SystemFunc {
private:
public:
	static TCHAR* concat(TCHAR* str1, TCHAR* str2);
	static TCHAR* getCurrDir();
	static TCHAR* readToCharFromFile(TCHAR* path);
	static void writeFromCharToFile(const TCHAR* str, TCHAR* fileName);
	static int ChToI(TCHAR ch);
	static int FormDigit(int nPos, TCHAR* str, double& res);
	static bool IsNum(TCHAR str);
	static int DigitToStr(double dig, TSTRING& res);
	static TCHAR IToCh(int a);
};