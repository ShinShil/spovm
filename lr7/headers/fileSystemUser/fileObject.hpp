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
	char contentDelim;

	int isEnd() {
		return content[pos] == closeDelim ? TRUE : FALSE;
	}
	int isBegin() {
		return content[pos] == openDelim ? TRUE : FALSE;
	}

	int isContentDelim() {
		return content[pos] == contentDelim ? TRUE : FALSE;
	}

	void missObjectContent() {
		if(content[pos] == openDelim) {
			StringService::jumpBlock(content, pos, openDelim, closeDelim);
		}
	}

	//return object where pos is set now, and move pos to the next obj
	string getNextObejct() {
		string obj = "";
		while(true) {
			if(isBegin()) {
				StringService::jumpBlock(content, pos, openDelim, closeDelim);
			}
			if(isContentDelim()) {
				++pos;
				return obj;
			}
			if(isEnd()) {
				pos = this->BEGIN_POS;
				return obj;
			}
			obj += content[pos];
		}
		if(obj[obj.length() - 1] == '$') {
			obj.pop_back();
		}
		return obj;
	}
	string fileObjectTypes[2];
public:
	void setEndPos() {
		int pos = BEGIN_POS;

		while(true) {
			missObjectContent();
			if(isEnd()) {
				END_POS = pos - 1;		
				return;
			}
			++pos;
		}
	}
	int getPos() {
		return pos;
	}
	void debug(int fContent = 0) {
		cout<<"[Info] pos: "<<pos<<"; content[pos]: "<<content[pos]<<"; BEG_POS: "<<BEGIN_POS<<endl;
		if(fContent) 
			cout<<"content: "<<content<<endl;
	}
	//file{content} begPos is set to 'c'
	FileObject(string content, string name, int begPos, char openDelim, char closeDelim) {
		cout<<"content: "<<content<<endl;
		this->content = content;
		this->name = name;
		this->BEGIN_POS = begPos;
		this->openDelim = openDelim;
		this->closeDelim = closeDelim;
		this->pos = begPos;
		this->fileObjectTypes[FILE_OBJECT_FILE] = "file";
		this->fileObjectTypes[FILE_OBJECT_DIRECTORY] = "directory";
	}
	vector<string> getAllObjects() {
		vector<string> res;
		pos = BEGIN_POS;
		do {
			res.push_back(getNextObejct());
		}while(pos != BEGIN_POS);
		return res;
	}
	//return objName{content} return pos for '{' + 1
	//if error returns 0
	int containsObject(string name) {
		int pos = BEGIN_POS;
		do {
			int oldPos = pos;
			if(name == getNextObejct()) {
				return oldPos;
			}
		}while(pos!=BEGIN_POS);
		return FALSE;
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
};
#endif