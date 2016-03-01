#include "systemFunc.h"

TCHAR* SystemFunc::concat(TCHAR* str1, TCHAR* str2) {
	#ifdef WIN32
	return _tcscat(str1, str2);
	#endif
	#ifdef __linux__
		return strcat(str1, str2);
	#endif
}

TCHAR* SystemFunc::getCurrDir() {
	TCHAR* currDir = new TCHAR[255];
#ifdef WIN32
	GetCurrentDirectory(255, currDir);
#endif
#ifdef __linux__ 
	getcwd(currDir, 255);
#endif
	return currDir;
}

TCHAR* SystemFunc::readToCharFromFile(TCHAR* path) {
	TCHAR* res = new TCHAR[255];
#ifdef WIN32
		OFSTRUCT of;
		HFILE hFile = OpenFile(path, &of, OF_READ);
		_lread(hFile, res, 255);
#endif
#ifdef __linux__
	ifstream in;
	in.open(path);
	in.getline(res, 255);
	in.close(); 
#endif
	return res;
}

void SystemFunc::writeFromCharToFile(const TCHAR* str, TCHAR* fileName) {
#ifdef WIN32
	OFSTRUCT of;
	HFILE hFile = OpenFile(fileName, &of, OF_CREATE);
	_lwrite(hFile, str, 255);
#endif
#ifdef __linux__
	ofstream out;
	out.open(fileName);
	out << str;
	out.close();
#endif
}

int SystemFunc::FormDigit(int nPos, TCHAR* str, double& res) {
	res = 0;
	while (IsNum(str[nPos])) {
		if (res)
			res *= 10;
		res += ChToI(str[nPos++]);
	}
	if (str[nPos] == '.') {
		++nPos;
		double ten = 0.1;
		while (IsNum(str[nPos])) {
			res += ChToI(str[nPos++]) * ten;
		}
	}
	return nPos;
}

int SystemFunc::ChToI(TCHAR ch) {
	switch (ch) {
	case '0':return 0;
	case '1':return 1;
	case '2':return 2;
	case '3':return 3;
	case '4':return 4;
	case '5':return 5;
	case '6':return 6;
	case '7':return 7;
	case '8':return 8;
	case '9':return 9;
	default:return -1;
	}
	return -2;
}


bool SystemFunc::IsNum(TCHAR ch) {
	if (ch >= '0' && ch <= '9') {
		return 1;
	}
	else
		return 0;
}

int SystemFunc::DigitToStr(double dig, TSTRING& res) {
	int ten = 1;
	int flagZnak = 0;
	if (dig < 0) {
		dig *= -1;
		flagZnak = 1;
	}
	double temp = dig;
	int nPoint = 0, nSize = 1;
	while (floor(temp) != temp) {
		temp *= 10;
		++nPoint;
	}
	while (temp / 10 >= 1) {
		temp /= 10;
		ten *= 10;
		++nSize;;
	}
	if (nPoint)
		++nSize;
	int temp2 = (int)(temp * ten);
	if (flagZnak)
		res += '-';
	for (int i = 0; i < nSize; ++i) {
		res += IToCh(temp2 / ten);
		temp2 %= ten;
		if (i == nPoint && i) {
			res += '.';
			++i;
		}
		ten /= 10;
	}
	return TRUE;
}


TCHAR SystemFunc::IToCh(int a) {
	switch (a) {
	case 0:return '0';
	case 1:return '1';
	case 2:return '2';
	case 3:return '3';
	case 4:return '4';
	case 5:return '5';
	case 6:return '6';
	case 7:return '7';
	case 8:return '8';
	case 9:return '9';
	default:return 'f';
	}
}
