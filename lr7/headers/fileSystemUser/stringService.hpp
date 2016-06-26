#pragma once
#include "main.h"

class StringService {
public:
	//jump to the pos where delim + 1, if it end go to front, jumpBlock because if skip Block {{}} ignore incapsulate }
	//pos should start at openDelim
	static void jumpBlock(string str, int& pos, char openDelim, char closeDelim) {
		stack<int> open;
		open.push(1);
		++pos;
		while(!open.empty()) {
			if(str[pos] == openDelim) {
				open.push(1);
			}
			if(str[pos] == closeDelim) {
				open.pop();
			}
			++pos;
		}
	}
	static void jumpBlockRev(string str, int& pos, char closeDelim, char openDelim) {
		stack<int> open;
		open.push(1);
		--pos;
		while(!open.empty()) {
			if(str[pos] == closeDelim) {
				open.push(1);
			}
			if(str[pos] == openDelim) {
				open.pop();
			}
			--pos;
		}	
	}

	static vector<string> commandSplit(string str) {
		int pos = 0;
		vector<string> res;
		if(str == "") {
			return res;
		}
		if(isEmpty(str[pos])) while(isEmpty(str[pos++]));
		string t = "";
		for(;pos<str.length(); ++pos) {
			if(isEmpty(str[pos]) && t!="") {
				res.push_back(t);
				t = "";
				continue;
			}
			t += str[pos];
		}
		if(t != "") {
			res.push_back(t);
		}
		return res;

	}

	static int isEmpty(char ch) {
		if(ch == ' ' || ch == '\t' || ch == '\n')
			return TRUE;
		else 
			return FALSE;
	}
};