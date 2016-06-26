#pragma once
#include "fileObject.hpp"

class FileUser : public FileObject {
public:
	virtual string getPath();	
	virtual string getName();
	virtual string getType();
	virtual string getContent();
}