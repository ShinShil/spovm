#pragma once
#include "main.h"
#include "fileObject.hpp"
#include "stringService.hpp"

class DirectoryUser : public FileObject {
public:
	DirectoryUser() {
		openDelim = '{';
		closeDelim = '{';
	}

	//if need to work at deep level, read all attributes to this object, work with them and than write all changes back
	void init() {

	}
	void writeChanges() {

	}
};