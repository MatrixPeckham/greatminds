/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextFileReader.h

	This is a wrapper class for reading text from a file. This
	can be useful for reading game design files for building a
	game world.
*/

#pragma once
#include "stdafx.h"

class TextFileReader
{
protected:
	// FILE NAME TO READ FROM
	wchar_t *fileName;

	// STREAM TO WRITE TO
	ifstream *inFile;

public:
	// METHODS DEFINED INSIDE TextFileReader
	TextFileReader();
	~TextFileReader();
	void closeReader();
	void initFile(wchar_t *initFileName);
	wchar_t* readLineOfText();	
}
;