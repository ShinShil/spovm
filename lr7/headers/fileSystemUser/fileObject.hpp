#pragma once
#include "main.h"
#include "debug.hpp"
#include "stringService.hpp"

#ifndef FILEOBJECT
#define FILEOBJECT

#define FILE_OBJECT_FILE 0
#define FILE_OBJECT_DIRECTORY 1
#define NOT_FOUND_FILE_OBJECT -1

class FileObject {
protected:
	int BEGIN_POS;
	int END_POS;
	int pos;

	string path;
	string content;
	string name;

	char openDelim;
	char closeDelim;
	char fileDelim;

	int isEnd() {
		return content[pos] == closeDelim ? TRUE : FALSE;
	}
	int isBegin() {
		return content[pos] == openDelim ? TRUE : FALSE;
	}
	int isFile(string name) {
		//Debug::debug(name.length() - 1, name + " - ");
		if(name[name.length() - 1] == fileDelim) {
			return TRUE;
		} else {
			return  FALSE;
		}
	}
	int isDir(string name) {
		if(isFile(name))
			return FALSE;
		else return TRUE;
	}
	void missObjectName() {
		while(content[pos]!=openDelim) ++pos;
	}
	void missObject() {
		if(content[pos]==closeDelim)
			return;
		while(content[pos++]!=openDelim);
		--pos;
		missObjectContent();
	}
	void missObjectContent() {
		if(content[pos] == openDelim) {
			StringService::jumpBlock(content, pos, openDelim, closeDelim);
		}
	}
	string getObjectName() {
		if(content[pos] == closeDelim)
			return "";
		int oldPos = pos;
		string res = "";
		while(content[pos]!=openDelim) {
			res += content[pos++];
		}
		pos = oldPos;
		return res;
	}

	//return object where pos is set now, and move pos to the next obj
	string getNextObject(int flag = 1) {
		string obj = "";
		if(isEnd()) {
			return "";
			pos = BEGIN_POS;
		}
		obj=getObjectName();
		missObjectName();
		StringService::jumpBlock(content, pos, openDelim, closeDelim);
		if(isEnd() && flag) {
			pos = this->BEGIN_POS;
		}
		return obj;
	}
	string fileObjectTypes[2];

public:
	//file{content} begPos is set to 'c'
	FileObject(string content, string name, int begPos, char openDelim, char closeDelim) {
		cout<<"content: "<<content<<endl;
		this->content = content;
		this->name = name;
		this->BEGIN_POS = begPos;
		this->openDelim = openDelim;
		this->closeDelim = closeDelim;
		this->fileDelim = '$';
		this->pos = begPos;
		this->fileObjectTypes[FILE_OBJECT_FILE] = "file";
		this->fileObjectTypes[FILE_OBJECT_DIRECTORY] = "directory";
	}
	void go(string dirName) {
		pos = BEGIN_POS;
		do {
			int oldPos = pos;
			string name = getNextObject();
			if(name == dirName) {
				while(content[oldPos++]!=openDelim);
				pos = oldPos;
				BEGIN_POS = oldPos;
				return;
			}
		}while(pos!=BEGIN_POS);
	}
	void goUp() {
		pos = BEGIN_POS;
		if(BEGIN_POS - 2 > 5) {
			pos = BEGIN_POS - 2;
			while(content[pos]!=openDelim) {
				if(content[pos] == closeDelim) {
					StringService::jumpBlockRev(content, pos, closeDelim, openDelim);
				}else {
					--pos;
				}
			}
			++pos;	
			BEGIN_POS = pos;			
		}

	}
	void setEndPos() {
		pos = BEGIN_POS - 1;
		StringService::jumpBlock(content, pos, openDelim, closeDelim);
		pos -= 2;		
	}

	int getPos() {
		return pos;
	}
	void debug(int fContent = 0) {
		cout<<"[fileObject] pos: "<<pos<<"; content[pos]: "<<content[pos]<<"; BEG_POS: "<<BEGIN_POS<<endl;
		if(fContent) 
			cout<<"content: "<<content<<endl;
	}
	int getObject(string name) {
		string dir = "";
		pos = BEGIN_POS;
		do {
			int oldPos = pos;
			dir = getNextObject();
			if(dir == name) {
				return oldPos;
			}
		}while(pos!=BEGIN_POS);	
	}
	vector<string> getObjects() {
		vector<string> res;
		pos = BEGIN_POS;
		do {
			string t = getNextObject();
			if(isFile(t)) {
				t.erase(t.length() - 1, 1);
			}
			res.push_back(t);
		}while(pos != BEGIN_POS);
		return res;
	}
	vector<string> getDirs() {
		vector<string> res;
		pos = BEGIN_POS;
		do {
			string t = getNextObject();
			if(isDir(t))  {
				res.push_back(t);
			}
		}while(pos != BEGIN_POS);
		return res;
	}
	vector<string> getFiles() {
		vector<string> res;
		pos = BEGIN_POS;
		do {
			string t = getNextObject();
			if(isFile(t)) {
				t.erase(t.length() - 1, 1);
				res.push_back(t);
			}
		}while(pos != BEGIN_POS);
		return res;
	}
	void addObj(string name) {
		setEndPos();
		++pos;
		content.insert(pos,name);
		pos = BEGIN_POS;
	}
	void remObj(string namerem) {
		int t = getObject(namerem);
		if(pos == BEGIN_POS) {
			setEndPos();
		}
		content.erase(t, pos - t);
		pos = BEGIN_POS;
	}
	int getBeginPos() {
		return BEGIN_POS;
	}
	void setFileContent(string fileName,string newContent) {
		int begPos = BEGIN_POS;
		go(fileName);
		int beg = this->pos;
		setEndPos();
		int end = this->pos;
		
		content.erase(beg, end-beg+1);
		content.insert(beg, newContent);
		BEGIN_POS = begPos;
		pos = BEGIN_POS;
	}
	string getFileContent(string fileName) {
	    string res;
		pos = BEGIN_POS;
		do {
			int oldPos = pos;
			string t = getNextObject();
			if(isFile(t) && t == fileName) {
				pos = oldPos;
				while(content[pos] != openDelim) ++pos;
				res = StringService::jumpBlock(content, pos, openDelim, closeDelim);
				debug();
				pos = BEGIN_POS;
				return res;
			}
		}while(pos != BEGIN_POS);
		return "";

	}
	//return objName{content} return pos for '{' + 1
	//if error returns 0
	int containsObject(string name) {
		pos = BEGIN_POS;
		do {
			int oldPos = pos;
			if(name == getNextObject()) {
				return oldPos;
			}
		}while(pos!=BEGIN_POS);
		return FALSE;
	}
	void setPos(int pos) {
		if(pos>0) {
			this->pos = pos;
		}
	}
	string getPath() {
		return path;
	}
	string getName() {
		return name;
	}
	
	string getContent() {
		return content;
	}
	void setBeginPos(int p) {
		BEGIN_POS = p;
	}
};
#endif