/*
FILE:		CVariable.hpp

REMARKS:	This file contains the class "CVaribale", which models a variable in Lisp.
*/
#pragma once

#include <iostream>

#include "CToken.hpp"
#include "Variables/TokenTypes.hpp"

using namespace std;



/**
* Instances of this class resemble variable's in Lisp.
* 
* @author	Christian-2003
* @version	12.07.2021
*/
class CVariable {
private:
	/**
	* Stores the name of the variable.
	*/
	string sName;

	/**
	* Stores the value of the variable as string.
	*/
	string sValue;

	/**
	* Stores the type of the variable's value.
	*/
	short int nType;



public:
	/**
	* The default constructor of this class creates a new variable with no name which stores an empty string.
	* IT IS NOT RECOMMENDET TO USE THIS CONSTRUCTOR.
	*/
	CVariable() {
		sName = "";
		sValue = "";
		nType = Token::U_STRING;
	}

	/**
	* The default constructor of this class creates a new variable with the passed values as attributes.
	* 
	* @param psName		Name of the variable.
	* @param psValue	Value of the variable.
	* @param pnType		Type of the variable's value.
	*/
	CVariable(string psName, string psValue, short int pnType) {
		sName = psName;
		sValue = psValue;
		nType = pnType;
	}



public:
	/**
	* Returns the name of the variable.
	* 
	* @return	Name of the variable.
	*/
	string getName() {
		return sName;
	}

	/**
	* Returns the variable's value as string.
	* 
	* @return	Variable's value.
	*/
	string getValue() {
		return sValue;
	}

	/**
	* Returns the value's type.
	* 
	* @return	Value's type.
	*/
	short int getType() {
		return nType;
	}



public:
	/**
	* Changes the value (and type) of the variable to the passed values.
	* 
	* @param psValue	New value of the variable.
	* @param pnType		Type of the new value.
	*/
	void changeValue(string psValue) {
		sValue = psValue;
	}
};
