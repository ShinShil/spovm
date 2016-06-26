#pragma once
#include "MainHeader.h"

class Service {
public:
	static void writeToFile(string src, string path);
	static void readFromFile(string& dest, string path);
	static string getCurrDir();
	static void cycleIncrement(int& i, int max);
    static int stringToInt(string num) {
        int res = 0;
        int ten = 1;
        for(int i = num.length() - 1; i>=0; --i) {
           int t = chToI(num[i]);
           t *= ten;
           res += t;
           ten *= 10; 
        }
        return res;
    }
    static string intToString(int num) {
        string res = "";
        while(num/10 > 0) {
            int t = num % 10;
            num /= 10;
            res += iToCh(t);
        }
        int t = num%10;
        res += iToCh(t);
        reverse(res.begin(), res.end());
        return res;       
    }
    static void readLineFromFile(string& dest, string path) {
        ifstream in;
    	in.open(path.c_str());
        in >> dest;
        //cout<<"debug readFile: "<<path <<" "<<dest<<endl;
	   in.close();
    }
    static char iToCh(int num) {
        switch(num) {
            case 1: return '1'; break;
            case 2: return '2'; break;
            case 3: return '3'; break;
            case 4: return '4'; break;
            case 5: return '5'; break;
            case 6: return '6'; break;
            case 7: return '7'; break;
            case 8: return '8'; break;
            case 9: return '9'; break;
            case 0: return '0'; break;
        }
    }
    static int chToI(char ch) {
        switch(ch) {
            case '1': return 1; break;
            case '2': return 2; break;
            case '3': return 3; break;
            case '4': return 4; break;
            case '5': return 5; break;
            case '6': return 6; break;
            case '7': return 7; break;
            case '8': return 8; break;
            case '9': return 9; break;
            case '0': return 0; break;
        }
    }
    static vector<string> splitLines(string line) {
        string t = "";
        vector<string> res;
        if(line != "")
            for(int i = 0; i<line.length(); ++i) {
                if(line[i]!='\n') {
                    t += line[i];
                }else {
                    res.push_back(t);
                    t = "";
                }
            }
            if(t!="")
                res.push_back(t);
        return res;
    }
    static void addLineToFile(string line, string path) {
        ofstream ofs;
        ofs.open(path.c_str(), ios_base::app);
        line += '\n';
        ofs << line;
        ofs.close();
    }
    static void sleep(int ms) {
        #ifdef WIN32
            Sleep(ms);
        #endif
        #ifdef __linux__
            sleep(ms);
        #endif
    }
};