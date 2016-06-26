#include "MainHeader.h"
#include "Service.h"

class StackFile {
private:
    string path;
public:
    StackFile(string path);
    void push(string str) {
        ofstream out;
        out.open(path.c_str());
        out << src << endl;
        out.close();
    }
    string pop() {
        ifstream in;
        in.open(path.c_str());
        string content;
        in >> content;
        in.close();
        
        if(content == "") return "";
        string res;
        int i;
        while(content[i] != '\n') {
            res += content[i];
            ++i;
        }
        content.erase(0, content, i);
        Service::writeToFile(string content, path);
                    
        return res;
    }
    
    string peek() {
        ifstream in;
        in.open(path.c_str());
        string content;
        in >> content;
        in.close();
        
        if(content == "") return "";
        string res;
        int i;
        while(content[i] != '\n') {
            res += content[i];
            ++i;
        }
        return res;
    }
};