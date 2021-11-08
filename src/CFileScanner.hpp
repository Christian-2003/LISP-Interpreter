/*
FILE:		CFileScanner.hpp

REMARKS:	This file contains the class "CFileScanner", which is used to read a file's content.
*/
#pragma once

#include <string>
#include <fstream>

#include "CRV.hpp"
#include "Variables/ErrorMessages.hpp"

using namespace std;



/**
* An object of this class can read a file's content and return said content as a String.
* 
* @author	Christian-2003
* @version	05.06.2021
*/
class CFileScanner {
public:
	/**
	* This functions reads the file "psFileName and returns it's content as string.
	* 
	* @param psFileName	Name of the file (including path and extension)
	* @return			Content of the file.
	*/
	CRV<string> scan(string psFileName) {
		fstream fileObj{ psFileName, ios::in };
		string sContent = ""; //Stores the file's content.
		if (!fileObj.is_open()) {
			return CRV<string>("", Error::FileScanner::ERROR_FILESCANNER_FILE_DOES_NOT_EXIST); //Return empty String.
		}

		//Read the file's content line by line:
		string sCurrentLine = "";
		while (getline(fileObj, sCurrentLine)) {
			sContent += sCurrentLine + '\n';
		}

		return CRV<string>(sContent, Error::SUCCESS);
	}
};
